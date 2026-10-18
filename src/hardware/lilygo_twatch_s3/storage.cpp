#include "hardware/storage.h"
#include <Preferences.h>
#include <algorithm>
#include <cstring>

int32_t get_storage_int(
	const char *space,
	const char *key,
	int32_t default_value
)
{
	Preferences preferences;
	if (!preferences.begin(space, true)) {
		return default_value;
	}
	const int32_t value = preferences.getInt(key, default_value);
	preferences.end();
	return value;
}

void set_storage_int(
	const char *space,
	const char *key,
	int32_t value
)
{
	Preferences preferences;
	if (!preferences.begin(space, false)) {
		return;
	}
	preferences.putInt(key, value);
	preferences.end();
}

bool get_storage_bool(
	const char *space,
	const char *key,
	bool default_value
)
{
	Preferences preferences;
	if (!preferences.begin(space, true)) {
		return default_value;
	}
	const bool value = preferences.getBool(key, default_value);
	preferences.end();
	return value;
}

void set_storage_bool(
	const char *space,
	const char *key,
	bool value
)
{
	Preferences preferences;
	if (!preferences.begin(space, false)) {
		return;
	}
	preferences.putBool(key, value);
	preferences.end();
}

size_t get_storage_string(
	const char *space,
	const char *key,
	char *buffer,
	size_t buffer_size,
	const char *default_value
)
{
	if (buffer_size == 0) {
		return 0;
	}
	String value(default_value);
	Preferences preferences;
	if (preferences.begin(space, true)) {
		value = preferences.getString(key, default_value);
		preferences.end();
	}
	const size_t length = std::min(static_cast<size_t>(value.length()), buffer_size - 1);
	std::memcpy(buffer, value.c_str(), length);
	buffer[length] = '\0';
	return length;
}

void set_storage_string(
	const char *space,
	const char *key,
	const char *value
)
{
	Preferences preferences;
	if (!preferences.begin(space, false)) {
		return;
	}
	preferences.putString(key, value);
	preferences.end();
}

size_t get_storage_bytes(
	const char *space,
	const char *key,
	void *buffer,
	size_t buffer_size
)
{
	Preferences preferences;
	if (!preferences.begin(space, true)) {
		return 0;
	}
	const size_t length = preferences.getBytes(key, buffer, buffer_size);
	preferences.end();
	return length;
}

void set_storage_bytes(
	const char *space,
	const char *key,
	const void *value,
	size_t size
)
{
	Preferences preferences;
	if (!preferences.begin(space, false)) {
		return;
	}
	preferences.putBytes(key, value, size);
	preferences.end();
}
