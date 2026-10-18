import re
import struct
import sys

LV_IMAGE_HEADER_MAGIC = 0x19

COLOR_FORMATS = {
    "LV_COLOR_FORMAT_RGB565": (0x12, 2),
    "LV_COLOR_FORMAT_RGB888": (0x0F, 3),
    "LV_COLOR_FORMAT_ARGB8888": (0x10, 4),
    "LV_COLOR_FORMAT_XRGB8888": (0x11, 4),
}


def main():
    if len(sys.argv) != 3:
        sys.exit("usage: image_header_to_bin.py <image.h> <image.bin>")

    source = open(sys.argv[1], encoding="utf-8").read()

    array = re.search(r"uint8_t\s+\w+\[\]\s*=\s*\{(.*?)\};", source, re.S)
    color_format = re.search(r"\.cf\s*=\s*(LV_COLOR_FORMAT_\w+)", source)
    width = re.search(r"\.w\s*=\s*(\d+)", source)
    height = re.search(r"\.h\s*=\s*(\d+)", source)
    if not (array and color_format and width and height):
        sys.exit(f"{sys.argv[1]}: image array or descriptor not found")

    if color_format.group(1) not in COLOR_FORMATS:
        sys.exit(f"{sys.argv[1]}: unsupported color format {color_format.group(1)}")

    cf, bytes_per_pixel = COLOR_FORMATS[color_format.group(1)]
    w = int(width.group(1))
    h = int(height.group(1))
    stride = w * bytes_per_pixel

    data = bytes(int(value, 16) for value in re.findall(r"0x[0-9a-fA-F]+", array.group(1)))
    if len(data) != stride * h:
        sys.exit(f"{sys.argv[1]}: data has {len(data)} bytes, expected {stride * h}")

    header = struct.pack("<BBHHHHH", LV_IMAGE_HEADER_MAGIC, cf, 0, w, h, stride, 0)
    with open(sys.argv[2], "wb") as output:
        output.write(header + data)


main()
