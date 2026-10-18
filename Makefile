PIO ?= $(shell command -v pio 2>/dev/null || echo $(HOME)/.platformio/penv/bin/pio)
ENV ?= twatchs3
CLANG_FORMAT ?= clang-format
RUN_CLANG_TIDY ?= run-clang-tidy
COMPILEDB_DIR = .pio/compiledb

FORMAT_SOURCES = $(shell find src \( -path src/core/ui/fonts -o -path src/core/structs -o -path src/emulator -o -type d -name images -o -type d -name icons \) -prune -o \( -name '*.cpp' -o -name '*.h' \) -print)
TIDY_FILTER = ^$(CURDIR)/src/(?!core/ui/fonts/|core/structs/|.*/images/|.*/icons/).*\.cpp$$

DOCKER_COMPOSE = HOST_UID=$(shell id -u) HOST_GID=$(shell id -g) docker compose
DOCKER_RUN = $(DOCKER_COMPOSE) run --rm synthwired

.PHONY: build buildfs size upload uploadfs upload-all emulator compiledb format-check docker-image docker-flasher

build:
	$(PIO) run -e $(ENV)

buildfs:
	$(PIO) run -e $(ENV) -t buildfs

size:
	$(PIO) run -e $(ENV) -t size

upload:
	$(PIO) run -e $(ENV) -t upload

uploadfs:
	$(PIO) run -e $(ENV) -t uploadfs

upload-all:
	$(PIO) run -e $(ENV) -t uploadfs
	$(PIO) run -e $(ENV) -t upload

emulator:
	$(PIO) run -e emulator -t exec

compiledb:
	$(PIO) run -e $(ENV) -t compiledb
	python3 scripts/clang_compile_commands.py compile_commands.json $(COMPILEDB_DIR)/compile_commands.json

format-check: compiledb
	@status=0; \
	$(CLANG_FORMAT) --dry-run --Werror $(FORMAT_SOURCES) || status=1; \
	awk '{ line = $$0; gsub(/\t/, "    ", line); if (length(line) > 120) { printf "%s:%d: error: line has %d characters, limit is 110\n", FILENAME, FNR, length(line); bad = 1 } } END { exit bad }' $(FORMAT_SOURCES) || status=1; \
	awk 'FNR == 1 { record = 0; expect = "" } expect == "blank" && $$0 !~ /^[ \t]*$$/ { printf "%s:%d: error: missing empty line after class opening brace\n", FILENAME, FNR; bad = 1 } expect == "content" && $$0 ~ /^[ \t]*$$/ { printf "%s:%d: error: empty line after opening brace\n", FILENAME, FNR; bad = 1 } { expect = "" } /^[ \t]*(class|struct)[ \t]+[A-Za-z_]/ && $$0 !~ /;[ \t]*$$/ { record = 1 } /^[ \t]*\{[ \t]*$$/ { expect = record ? "blank" : "content"; record = 0; next } /\{[ \t]*$$/ { expect = "content"; record = 0 } END { exit bad }' $(FORMAT_SOURCES) || status=1; \
	python3 scripts/check_fs_paths.py src data || status=1; \
	python3 scripts/check_includes.py src $(FORMAT_SOURCES) || status=1; \
	tidy_output=$$($(RUN_CLANG_TIDY) -p $(COMPILEDB_DIR) -quiet -extra-arg=-Wno-unused-command-line-argument '$(TIDY_FILTER)' 2>&1); \
	printf '%s\n' "$$tidy_output" | grep -vE '^(clang-tidy|[0-9]+ warnings? (generated|treated as errors)|$$)' | awk '/^[^ :]+:[0-9]+:[0-9]+: (error|warning):/ { skip = ($$0 ~ /^(.*\/)?\.pio\//); bad = bad || !skip } !skip { print } END { exit bad }' || status=1; \
	exit $$status

docker-image:
	$(DOCKER_COMPOSE) build

docker-flasher:
	$(DOCKER_COMPOSE) up

docker-%:
	$(DOCKER_RUN) make $* ENV=$(ENV)
