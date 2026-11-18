import json
import os
import shlex
import subprocess
import sys

UNSUPPORTED_FLAGS = {
    "-mlongcalls",
    "-mdisable-hardware-atomics",
    "-fstrict-volatile-bitfields",
    "-fno-tree-switch-conversion",
    "-Werror=return-type",
}


def get_arguments(entry):
    if "arguments" in entry:
        return entry["arguments"]
    return shlex.split(entry["command"])


def get_system_includes(compiler):
    result = subprocess.run(
        [compiler, "-E", "-x", "c++", "-", "-v"],
        input="",
        capture_output=True,
        text=True,
    )
    lines = result.stderr.splitlines()
    start = lines.index("#include <...> search starts here:") + 1
    end = lines.index("End of search list.")
    includes = [os.path.normpath(line.strip()) for line in lines[start:end]]
    return [include for include in includes if "/lib/gcc/" not in include]


def get_clang_entry(entry, system_includes):
    arguments = get_arguments(entry)
    clang_arguments = [
        "clang++",
        "--target=i686-unknown-elf",
        "-D__XTENSA__",
        "-nostdlibinc",
    ]
    for include in system_includes:
        clang_arguments += ["-isystem", include]
    clang_arguments += [argument for argument in arguments[1:] if argument not in UNSUPPORTED_FLAGS]
    return {
        "directory": entry["directory"],
        "file": entry["file"],
        "arguments": clang_arguments,
    }


def is_project_source(entry):
    path = os.path.relpath(os.path.join(entry["directory"], entry["file"]), entry["directory"])
    return path.startswith("src/")


source_path = sys.argv[1]
target_path = sys.argv[2]

with open(source_path) as source_file:
    entries = [entry for entry in json.load(source_file) if is_project_source(entry)]

system_includes = get_system_includes(get_arguments(entries[0])[0])

os.makedirs(os.path.dirname(target_path), exist_ok=True)
with open(target_path, "w") as target_file:
    json.dump([get_clang_entry(entry, system_includes) for entry in entries], target_file, indent=1)
