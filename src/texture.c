#include "texture.h"
#include "stb_image.h"


texture* texture_from_file(const char* file) {
	texture* this = malloc(sizeof(texture));
	glGenTextures(1, &this->handle);
	stbi_set_flip_vertically_on_load(1);
	unsigned char* img_data = load_image(file, &this->width, &this->height, &this->channels);

	glBindTexture(GL_TEXTURE_2D, this->handle);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (img_data) {
		glTexImage2D(GL_TEXTURE_2D, 0, this->channels == 3 ? GL_RGB : GL_RGBA, this->width, this->height, 0, this->channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, img_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(img_data);

	return this;
}

void texture_bind(texture* this) {
	glBindTexture(GL_TEXTURE_2D, this->handle);
}

void texture_delete(texture* this) {
	glDeleteTextures(1, &this->handle);
}