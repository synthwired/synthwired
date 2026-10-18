#include "hardware/filesystem.h"
#include <FFat.h>
#include <esp_heap_caps.h>

void filesystem_init()
{
	FFat.begin(false);
}

uint8_t *get_filesystem_file(const char *path, size_t &size)
{
	size = 0;
	File file = FFat.open(path, "r");
	if (!file) {
		return nullptr;
	}
	const size_t fileSize = file.size();
	uint8_t *const data = static_cast<uint8_t *>(heap_caps_malloc(fileSize, MALLOC_CAP_SPIRAM));
	if (data == nullptr) {
		file.close();
		return nullptr;
	}
	if (file.read(data, fileSize) != fileSize) {
		heap_caps_free(data);
		file.close();
		return nullptr;
	}
	file.close();
	size = fileSize;
	return data;
}

void filesystem_free_file(uint8_t *data)
{
	heap_caps_free(data);
}
