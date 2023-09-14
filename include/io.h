#ifndef IO_H
#define IO_H


#include <memory.h>
#include <malloc.h>
#include <stdio.h>

char* load_file(const char* path);

unsigned char* load_image(const char* path, int* w, int* h, int* channels);

#endif