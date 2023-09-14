#include "io.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

char* load_file(const char* path) {
		FILE* fp;
		long size = 0;
		char* data;
		fopen_s(&fp, path, "rb");
		if (fp == NULL) {
			return "";
		}
		fseek(fp, 0L, SEEK_END);
		size = ftell(fp);
		rewind(fp);
		data = malloc(size + 1);
		data[size] = 0;
		fread(data, 1, size, fp);
		fclose(fp);

		return data;
}

unsigned char* load_image(const char* path, int* w, int* h, int* channels) {
	unsigned char* img = stbi_load(path, w, h, channels, NULL);
	return img;
}