import os
import re
import sys

STANDARD_HEADERS = {
    "algorithm": r"\bstd::(min|max|clamp|sort|find|find_if|fill|copy|count|reverse|any_of|all_of)\b",
    "chrono": r"\bstd::chrono\b",
    "cmath": (
        r"\bstd::(abs|fabs|fmod|log|log10|exp|isfinite|isnan|sqrt|sin|cos|tan|asin|acos|atan|atan2"
        r"|hypot|round|lround|floor|ceil|pow)\b|\b(sinf|cosf|tanf|sqrtf|fabsf|roundf|atan2f|M_PI)\b"
    ),
    "cstddef": r"\b(size_t|nullptr_t|ptrdiff_t)\b",
    "cstdint": r"\bu?int(8|16|32|64)_t\b",
    "cstdio": r"\b(snprintf|sprintf|printf|sscanf|fopen|fclose|fread|fwrite|fseek|ftell|FILE)\b",
    "cstdlib": r"\b(atoi|atol|atof|strtol|strtoul|strtod|malloc|calloc|realloc|free|exit|abs|rand|srand)\b",
    "cstring": r"\b(strlen|strcmp|strncmp|strchr|strrchr|strstr|strcpy|strncpy|strcat|memcpy|memmove|memset|memcmp)\b",
    "ctime": r"\b(time_t|tm|time|localtime_r|mktime)\b",
    "fstream": r"\bstd::(ifstream|ofstream|fstream)\b",
    "map": r"\bstd::map\b",
    "memory": r"\b(unique_ptr|make_unique|shared_ptr|make_shared)\b",
    "string": r"\bstd::(string|to_string|getline)\b",
    "thread": r"\bstd::this_thread\b",
    "type_traits": r"\bstd::is_\w+",
    "unordered_map": r"\bstd::unordered_map\b",
}

LIBRARY_HEADERS = {
    "lvgl.h": r"\b(lv_\w+|LV_\w+)",
    "LilyGoLib.h": r"\binstance\.|\b(radio|DeviceEvent_t|SensorBMA423|DISP_BL)\b|\b(PMU|SENSOR|RTC|TP)_\w+",
    "Arduino.h": r"\b(Serial|delay|millis|String|btStop)\b",
    "LV_Helper.h": r"\bbeginLvglHelper\b",
    "Wire.h": r"\bWire\.",
    "Preferences.h": r"\bPreferences\b",
    "WiFi.h": r"\bWiFi\.",
    "FFat.h": r"\bFFat\b",
    "esp_heap_caps.h": r"\bheap_caps_\w+",
    "driver/ledc.h": r"\bledc_\w+|\bLEDC_\w+",
    "esp32-hal-periman.h": r"\bperiman\w+",
    "esp_pm.h": r"\besp_pm_\w+",
    "esp_sleep.h": r"\besp_sleep_\w+",
    "esp_freertos_hooks.h": r"\besp_register_freertos_\w+",
    "SDL2/SDL.h": r"\bSDL_\w+",
}

IGNORED_NAMES = {"if", "for", "while", "switch", "return", "sizeof", "static_assert", "defined", "static_cast"}

FUNCTION_PATTERN = re.compile(
    r"^\s*(?:(?:static|virtual|inline|constexpr|extern)\s+)*(?:const\s+)?[A-Za-z_][\w:<>]*[\s\*&]+(\w+)\s*\(",
    re.M,
)
TYPE_PATTERN = re.compile(r"\b(?:class|struct|enum(?:\s+class)?)\s+(\w+)")
MACRO_PATTERN = re.compile(r"#define\s+(\w+)")
VARIABLE_PATTERN = re.compile(r"\b(?:constexpr|extern)\s+[\w:<>\s\*]*?\b(\w+)\s*(?:\[|=|;|\{)")
INCLUDE_PATTERN = re.compile(r'^\s*#include\s*([<"])([^>"]+)[>"]', re.M)


def strip_comments(text):
    text = re.sub(r"//[^\n]*", "", text)
    return re.sub(r"/\*.*?\*/", "", text, flags=re.S)


def read(path):
    with open(path, encoding="utf-8", errors="ignore") as file:
        return file.read()


def get_declared_names(path):
    text = strip_comments(read(path))
    names = set()
    names.update(TYPE_PATTERN.findall(text))
    names.update(MACRO_PATTERN.findall(text))
    names.update(FUNCTION_PATTERN.findall(text))
    names.update(VARIABLE_PATTERN.findall(text))
    return {name for name in names - IGNORED_NAMES if not name.startswith("lv_")}


def get_include_path(include, path, source_dir):
    for base in (os.path.dirname(path), source_dir):
        candidate = os.path.normpath(os.path.join(base, include))
        if os.path.isfile(candidate):
            return candidate
    return None


def is_used(kind, include, path, body, source_dir):
    if kind == "<":
        pattern = STANDARD_HEADERS.get(include) or LIBRARY_HEADERS.get(include)
        return pattern is None or re.search(pattern, body) is not None
    target = get_include_path(include, path, source_dir)
    if target is None or target == os.path.splitext(path)[0] + ".h":
        return True
    return any(re.search(r"\b" + re.escape(name) + r"\b", body) for name in get_declared_names(target))


def main():
    source_dir = os.path.normpath(sys.argv[1])
    bad = False
    for path in sys.argv[2:]:
        text = read(path)
        body = strip_comments(re.sub(r"^\s*#include[^\n]*", "", text, flags=re.M))
        for match in INCLUDE_PATTERN.finditer(text):
            kind, include = match.groups()
            if is_used(kind, include, os.path.normpath(path), body, source_dir):
                continue
            line = text.count("\n", 0, match.start()) + 1
            closing = ">" if kind == "<" else '"'
            print(f"{path}:{line}: error: unused include {kind}{include}{closing}")
            bad = True
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
