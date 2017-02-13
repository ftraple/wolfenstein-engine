/*
 * Texture.c
 *
 *  Created on: 3 de fev de 2017
 *      Author: fabiano
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Defs.h"
#include "Texture.h"

typedef  struct {
	bool InUse;
	char*filename;
	SDL_Surface*textureSurface;
} TextureContext;

TextureContext*pvTextureList = NULL;
uint16_t pvTextureListSize = 0;
uint16_t pvTextureListInUse = 0;

// Return the texture index
uint16_t OpenTexture(const char*filename) {

	uint16_t index = 0;

	// Allocate texture list memory if needed.
	if (pvTextureList == NULL) {
		pvTextureListSize = 100;
		pvTextureList = (TextureContext*)malloc(pvTextureListSize*sizeof(TextureContext));
		if (pvTextureList == NULL) {
			printf( "Allocate memory fail!\n");
			return 0;
		}
		// Solve BUG ?
		memset(pvTextureList, 0, pvTextureListSize*sizeof(TextureContext));
	}
	else if (pvTextureListInUse == pvTextureListSize) {
		pvTextureListSize += 100;
		TextureContext*textureListTemp = (TextureContext*)realloc( pvTextureList,pvTextureListSize*sizeof(TextureContext));
		if (textureListTemp == NULL) {
			printf( "Allocate memory fail!\n");
			return 0;
		}
		pvTextureList = textureListTemp;
	}

	// Find empty structure in the list.
	while (pvTextureList[index].InUse == TRUE) {
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
	pvTextureList[index].InUse = TRUE;
	pvTextureListInUse++;

	return index;
}

SDL_Surface*GetTextureSurface(uint16_t index) {

	// Check if texture index number is valid
	if (index >= pvTextureListSize) {
		printf( "Index number is too big! [%d].\n", index);
		return NULL;
	}
    if (pvTextureList[index].InUse == FALSE) {
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
    if (pvTextureList[index].InUse == FALSE) {
		printf( "The index number [%d] is not in use.\n", index);
		return;
    }

	SDL_FreeSurface(pvTextureList[index].textureSurface);
	pvTextureList[index].InUse = FALSE;
	pvTextureListInUse--;

}


