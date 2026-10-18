import glob
import json
import mimetypes
import os
import re
import struct
import subprocess
import threading
from http import HTTPStatus
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import unquote

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_DIR = os.path.dirname(SCRIPT_DIR)
HTML_DIR = os.path.realpath(os.path.join(SCRIPT_DIR, "html"))
ENVIRONMENT = os.environ.get("FLASHER_ENV", "twatchs3")
BUILD_DIR = os.path.join(PROJECT_DIR, ".pio", "build", ENVIRONMENT)
ESPTOOL_JS = os.environ.get("ESPTOOL_JS", "/opt/esptool-js/bundle.js")
PLATFORMIO_DIR = os.environ.get("PLATFORMIO_CORE_DIR", os.path.expanduser("~/.platformio"))
HOST = os.environ.get("FLASHER_HOST", "127.0.0.1")
PORT = int(os.environ.get("FLASHER_PORT", "8080"))

ALLOWED_HOSTNAMES = {"localhost", "127.0.0.1", "[::1]"}

PARTITION_ENTRY = struct.Struct("<HBBII16sI")
PARTITION_MAGIC = 0x50AA
PARTITION_TYPE_APP = 0x00
PARTITION_TYPE_DATA = 0x01
PARTITION_SUBTYPE_FAT = 0x81

MAX_DECODE_SIZE = 64 * 1024
CODE_ADDRESS = re.compile(r"0x4[0-9a-fA-F]{7}")

BUILD_COMMANDS = {
    "firmware": ["make", "build", f"ENV={ENVIRONMENT}"],
    "filesystem": ["make", "buildfs", f"ENV={ENVIRONMENT}"],
}

build_lock = threading.Lock()


def get_partitions():
    path = os.path.join(BUILD_DIR, "partitions.bin")
    if not os.path.isfile(path):
        return []
    with open(path, "rb") as partitions_file:
        data = partitions_file.read()
    partitions = []
    for offset in range(0, len(data) - PARTITION_ENTRY.size + 1, PARTITION_ENTRY.size):
        magic, kind, subtype, address, size, label, _ = PARTITION_ENTRY.unpack_from(data, offset)
        if magic != PARTITION_MAGIC:
            break
        partitions.append({
            "type": kind,
            "subtype": subtype,
            "address": address,
            "size": size,
            "label": label.rstrip(b"\0").decode("ascii", "replace"),
        })
    return partitions


def get_partition(partitions, kind, subtype=None):
    for partition in partitions:
        if partition["type"] == kind and (subtype is None or partition["subtype"] == subtype):
            return partition
    return None


def get_image(file_name, address, capacity):
    path = os.path.join(BUILD_DIR, file_name)
    if not os.path.isfile(path):
        return {"file": file_name, "available": False, "error": "not built"}
    if address is None:
        return {"file": file_name, "available": False, "error": "partition not found"}
    size = os.path.getsize(path)
    if size > capacity:
        return {"file": file_name, "available": False, "error": "larger than partition"}
    return {
        "file": file_name,
        "available": True,
        "address": address,
        "size": size,
        "capacity": capacity,
        "modified": os.path.getmtime(path),
    }


def get_images():
    partitions = get_partitions()
    app = get_partition(partitions, PARTITION_TYPE_APP)
    fat = get_partition(partitions, PARTITION_TYPE_DATA, PARTITION_SUBTYPE_FAT)
    return {
        "firmware": get_image(
            "firmware.bin",
            app["address"] if app else None,
            app["size"] if app else 0,
        ),
        "filesystem": get_image(
            "fatfs.bin",
            fat["address"] if fat else None,
            fat["size"] if fat else 0,
        ),
        "factory": get_image(
            "firmware.factory.bin",
            0 if app else None,
            app["address"] + app["size"] if app else 0,
        ),
    }


def get_addr2line():
    candidates = sorted(glob.glob(os.path.join(
        PLATFORMIO_DIR,
        "packages",
        "*",
        "bin",
        "xtensa-esp32s3-elf-addr2line",
    )))
    return candidates[0] if candidates else None


def get_hostname(host):
    if host.startswith("["):
        return host.split("]", 1)[0] + "]"
    return host.split(":", 1)[0]


class FlasherHandler(BaseHTTPRequestHandler):
    server_version = "SynthWiredFlasher"

    def do_GET(self):
        if not self.is_allowed_request():
            self.send_error(HTTPStatus.FORBIDDEN)
            return
        path = unquote(self.path.split("?", 1)[0])
        if path == "/api/images":
            self.send_json(get_images())
        elif path.startswith("/api/images/"):
            self.send_image(path[len("/api/images/"):])
        elif path == "/vendor/esptool-js.js":
            self.send_file(ESPTOOL_JS)
        else:
            self.send_static(path)

    def do_POST(self):
        if not self.is_allowed_request():
            self.send_error(HTTPStatus.FORBIDDEN)
            return
        path = unquote(self.path.split("?", 1)[0])
        if path.startswith("/api/build/"):
            self.run_build(path[len("/api/build/"):])
        elif path == "/api/decode":
            self.decode_backtrace()
        else:
            self.send_error(HTTPStatus.NOT_FOUND)

    def is_allowed_request(self):
        host = self.headers.get("Host", "")
        if get_hostname(host) not in ALLOWED_HOSTNAMES:
            return False
        origin = self.headers.get("Origin")
        return origin is None or origin == f"http://{host}"

    def send_json(self, value):
        body = json.dumps(value).encode("utf-8")
        self.send_body(body, "application/json")

    def send_text(self, text):
        self.send_body(text.encode("utf-8"), "text/plain; charset=utf-8")

    def send_body(self, body, content_type):
        self.send_response(HTTPStatus.OK)
        self.send_header("Content-Type", content_type)
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self.send_header("X-Content-Type-Options", "nosniff")
        self.end_headers()
        self.wfile.write(body)

    def send_file(self, path, content_type=None):
        if not os.path.isfile(path):
            self.send_error(HTTPStatus.NOT_FOUND)
            return
        if content_type is None:
            content_type = "text/javascript" if path.endswith(".js") else mimetypes.guess_type(path)[0]
        with open(path, "rb") as source:
            body = source.read()
        self.send_body(body, content_type or "application/octet-stream")

    def send_static(self, path):
        full_path = os.path.realpath(os.path.join(HTML_DIR, path.lstrip("/")))
        if os.path.commonpath([full_path, HTML_DIR]) != HTML_DIR:
            self.send_error(HTTPStatus.NOT_FOUND)
            return
        if os.path.isdir(full_path):
            full_path = os.path.join(full_path, "index.html")
        self.send_file(full_path)

    def send_image(self, name):
        image = get_images().get(name)
        if image is None or not image["available"]:
            self.send_error(HTTPStatus.NOT_FOUND)
            return
        self.send_file(os.path.join(BUILD_DIR, image["file"]), "application/octet-stream")

    def run_build(self, target):
        command = BUILD_COMMANDS.get(target)
        if command is None:
            self.send_error(HTTPStatus.NOT_FOUND)
            return
        if not build_lock.acquire(blocking=False):
            self.send_error(HTTPStatus.CONFLICT, "Build already running")
            return
        try:
            self.send_response(HTTPStatus.OK)
            self.send_header("Content-Type", "text/plain; charset=utf-8")
            self.send_header("Cache-Control", "no-store")
            self.send_header("X-Content-Type-Options", "nosniff")
            self.end_headers()
            process = subprocess.Popen(
                command,
                cwd=PROJECT_DIR,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                env=dict(os.environ, PYTHONUNBUFFERED="1"),
            )
            try:
                for line in process.stdout:
                    self.wfile.write(line)
                    self.wfile.flush()
                code = process.wait()
                self.wfile.write(f"\n[exit {code}]\n".encode("utf-8"))
            except (BrokenPipeError, ConnectionResetError):
                process.kill()
                process.wait()
        finally:
            build_lock.release()

    def decode_backtrace(self):
        try:
            length = int(self.headers.get("Content-Length", "0"))
        except ValueError:
            length = 0
        if length <= 0 or length > MAX_DECODE_SIZE:
            self.send_error(HTTPStatus.BAD_REQUEST)
            return
        text = self.rfile.read(length).decode("utf-8", "replace")
        addresses = list(dict.fromkeys(CODE_ADDRESS.findall(text)))
        elf = os.path.join(BUILD_DIR, "firmware.elf")
        addr2line = get_addr2line()
        if not addresses or addr2line is None or not os.path.isfile(elf):
            self.send_text("")
            return
        result = subprocess.run(
            [addr2line, "-pfiaC", "-e", elf, *addresses],
            capture_output=True,
            text=True,
            timeout=10,
        )
        self.send_text(result.stdout)


def main():
    server = ThreadingHTTPServer((HOST, PORT), FlasherHandler)
    print(f"SynthWired flasher on http://localhost:{PORT}", flush=True)
    server.serve_forever()


if __name__ == "__main__":
    main()
