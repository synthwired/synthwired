#pragma once
#include <cstddef>
#include <cstdint>

void filesystem_init();

uint8_t *get_filesystem_file(const char *path, size_t &size);

void filesystem_free_file(uint8_t *data);
