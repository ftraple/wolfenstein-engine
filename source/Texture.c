
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Defs.h"
#include "Texture.h"

typedef  struct {
	bool inUse;
	char*filename;
	SDL_Surface*textureSurface;
} Texture;

Texture*pvTextureList = NULL;
uint16_t pvTextureListSize = 0;
uint16_t pvTextureListinUse = 0;

// Return the texture index
uint16_t OpenTexture(const char*filename) {

	uint16_t index = 0;

	// Allocate texture list memory if needed.
	if (pvTextureList == NULL) {
		pvTextureListSize = 100;
		pvTextureList = (Texture*)malloc(pvTextureListSize*sizeof(Texture));
		if (pvTextureList == NULL) {
			printf( "Allocate memory fail!\n");
			return 0;
		}
		memset(pvTextureList, 0, pvTextureListSize*sizeof(Texture));
	}
	else if (pvTextureListinUse == pvTextureListSize) {
		pvTextureListSize += 100;
		Texture*textureListTemp = (Texture*)realloc(pvTextureList,pvTextureListSize*sizeof(Texture));
		if (textureListTemp == NULL) {
			printf( "Allocate memory fail!\n");
			return 0;
		}
		pvTextureList = textureListTemp;
	}

	// Find empty structure in the list.
	while (pvTextureList[index].inUse == true) {
		index++;
	}

	// Fill the texture structure.

	//Load image at specified path
	SDL_Surface*textureSurfaceTemp = IMG_Load(filename);
	if (textureSurfaceTemp == NULL) {
		printf( "Unable to load image %s! SDL_image Error: %s\n", filename, IMG_GetError() );
		return -1;
	}
	//Convert surface to screen format
	pvTextureList[index].textureSurface = SDL_ConvertSurfaceFormat(textureSurfaceTemp, SDL_PIXELFORMAT_RGBA8888, 0);
	// Delete temporary loaded surface.
	SDL_FreeSurface(textureSurfaceTemp);
	// Check if texture surface was converted.
	if (pvTextureList[index].textureSurface == NULL) {
		printf( "Unable to optimize image %s! SDL Error: %s\n", filename, SDL_GetError() );
		return -1;
	}
	pvTextureList[index].filename = strdup(filename);
	pvTextureList[index].inUse = true;
	pvTextureListinUse++;

	return index;
}

SDL_Surface*GetTextureSurface(uint16_t index) {

	// Check if texture index number is valid
	if (index >= pvTextureListSize) {
		printf( "Index number is too big! [%d].\n", index);
		return NULL;
	}
    if (pvTextureList[index].inUse == false) {
		printf( "The index number [%d] is not in use.\n", index);
		return NULL;
    }

    return pvTextureList[index].textureSurface;
}

void CloseTexture(uint16_t index) {

	// Check if texture index number is valid
	if (index >= pvTextureListSize) {
		printf( "Index number is too big! [%d].\n", index);
		return;
	}
    if (pvTextureList[index].inUse == false) {
		printf( "The index number [%d] is not in use.\n", index);
		return;
    }

	SDL_FreeSurface(pvTextureList[index].textureSurface);
	pvTextureList[index].inUse = false;
	pvTextureListinUse--;

}


