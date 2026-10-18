#pragma once
#include <cstddef>
#include <cstdint>

int32_t get_storage_int(
	const char *space,
	const char *key,
	int32_t default_value
);

void set_storage_int(
	const char *space,
	const char *key,
	int32_t value
);

bool get_storage_bool(
	const char *space,
	const char *key,
	bool default_value
);

void set_storage_bool(
	const char *space,
	const char *key,
	bool value
);

size_t get_storage_string(
	const char *space,
	const char *key,
	char *buffer,
	size_t buffer_size,
	const char *default_value
);

void set_storage_string(
	const char *space,
	const char *key,
	const char *value
);

size_t get_storage_bytes(
	const char *space,
	const char *key,
	void *buffer,
	size_t buffer_size
);

void set_storage_bytes(
	const char *space,
	const char *key,
	const void *value,
	size_t size
);
