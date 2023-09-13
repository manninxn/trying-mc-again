#include "io.h"

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