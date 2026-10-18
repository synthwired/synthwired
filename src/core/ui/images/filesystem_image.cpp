#include "filesystem_image.h"
#include <cstring>
#include <string>
#include <unordered_map>
#include "hardware/filesystem.h"

static std::unordered_map<std::string, lv_image_dsc_t *> cached_images;

static lv_image_dsc_t *get_filesystem_image_from_file(const char *path)
{
	size_t size = 0;
	uint8_t *const file = get_filesystem_file(path, size);
	if (file == nullptr) {
		return nullptr;
	}
	lv_image_header_t header;
	if (size < sizeof(header)) {
		filesystem_free_file(file);
		return nullptr;
	}
	std::memcpy(&header, file, sizeof(header));
	if (
		header.magic != LV_IMAGE_HEADER_MAGIC
		|| size - sizeof(header) < static_cast<size_t>(header.stride) * header.h
	) {
		filesystem_free_file(file);
		return nullptr;
	}
	lv_image_dsc_t *const image = new lv_image_dsc_t{};
	image->header = header;
	image->data_size = size - sizeof(header);
	image->data = file + sizeof(header);
	return image;
}

lv_image_dsc_t *get_filesystem_image(const char *path)
{
	const std::unordered_map<std::string, lv_image_dsc_t *>::iterator cached = cached_images.find(path);
	if (cached != cached_images.end()) {
		return cached->second;
	}
	lv_image_dsc_t *const image = get_filesystem_image_from_file(path);
	if (image != nullptr) {
		cached_images[path] = image;
	}
	return image;
}
