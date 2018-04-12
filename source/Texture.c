

#include "Texture.h"

/*################################################################################*/
w3d_Texture* w3d_Texture_CretateFromRaw(const char* name, uint16_t width, uint16_t height, uint32_t* pixelDataABGR) {

	w3d_Texture* texture = (w3d_Texture*)malloc(sizeof(w3d_Texture));
	if (texture == NULL) {
		return NULL;
	}
	texture->name = strdup(name);
	texture->width = width;
	texture->height = height;
	texture->pixelDataSize = width*height;
	texture->pixelData = malloc(texture->pixelDataSize*sizeof(uint32_t));
	if (texture->pixelData == NULL) {
		free(texture);
		return NULL;
	}
	memcpy(texture->pixelData, pixelDataABGR, texture->pixelDataSize*sizeof(uint32_t));

	return texture;
}

/*################################################################################*/
void w3d_Texture_Destroy(w3d_Texture* texture) {
	assert(texture != NULL);
	free(texture->name);
	free(texture->pixelData);
	free(texture);
}
