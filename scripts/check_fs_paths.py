import os
import re
import sys

PATH_PATTERN = re.compile(r'"(/[^"\s]+\.[A-Za-z0-9]+)"')
SOURCE_EXTENSIONS = (".cpp", ".h")


def get_source_files(source_dir):
    for root, _, files in os.walk(source_dir):
        for name in sorted(files):
            if name.endswith(SOURCE_EXTENSIONS):
                yield os.path.join(root, name)


def get_data_files(data_dir):
    for root, _, files in os.walk(data_dir):
        for name in sorted(files):
            yield "/" + os.path.relpath(os.path.join(root, name), data_dir)


def get_references(source_dir):
    for source in get_source_files(source_dir):
        with open(source, encoding="utf-8", errors="replace") as source_file:
            for line_number, line in enumerate(source_file, 1):
                for match in PATH_PATTERN.finditer(line):
                    yield source, line_number, match.start() + 1, match.group(1)


source_dir = sys.argv[1]
data_dir = sys.argv[2]

failed = False
referenced = set()
for source, line_number, column, path in get_references(source_dir):
    referenced.add(path)
    if not os.path.isfile(data_dir + path):
        print(f"{source}:{line_number}:{column}: error: {data_dir}{path} does not exist")
        failed = True

for path in get_data_files(data_dir):
    if path not in referenced:
        print(f"{data_dir}{path}: error: file is not referenced from {source_dir}")
        failed = True

sys.exit(1 if failed else 0)
