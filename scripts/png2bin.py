import os
import struct
import sys

from PIL import Image

LV_IMAGE_HEADER_MAGIC = 0x19
LV_COLOR_FORMAT_RGB565 = 0x12
LV_COLOR_FORMAT_ARGB8888 = 0x10

DATA_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "data")


def has_alpha(image):
    if image.mode in ("RGBA", "LA", "PA"):
        return True
    return "transparency" in image.info


def get_rgb565(image):
    data = bytearray()
    for r, g, b in image.convert("RGB").getdata():
        data += struct.pack("<H", ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3))
    return LV_COLOR_FORMAT_RGB565, 2, bytes(data)


def get_argb8888(image):
    return LV_COLOR_FORMAT_ARGB8888, 4, image.convert("RGBA").tobytes("raw", "BGRA")


def main():
    if len(sys.argv) != 2:
        sys.exit("usage: png2bin.py <image.png>")

    source = sys.argv[1]
    image = Image.open(source)
    w, h = image.size

    if has_alpha(image):
        cf, bytes_per_pixel, data = get_argb8888(image)
    else:
        cf, bytes_per_pixel, data = get_rgb565(image)

    header = struct.pack("<BBHHHHH", LV_IMAGE_HEADER_MAGIC, cf, 0, w, h, w * bytes_per_pixel, 0)

    name = os.path.splitext(os.path.basename(source))[0] + ".bin"
    os.makedirs(DATA_DIR, exist_ok=True)
    output = os.path.normpath(os.path.join(DATA_DIR, name))
    with open(output, "wb") as file:
        file.write(header + data)

    print(f"{output}: {w}x{h} {'ARGB8888' if cf == LV_COLOR_FORMAT_ARGB8888 else 'RGB565'}")


main()
