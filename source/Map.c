/*
 * Map.c
 *
 *  Created on: 3 de fev de 2017
 *      Author: fabiano
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Map.h"
#include "Texture.h"

struct MapContext {
	char*filename;
	uint16_t width;
	uint16_t height;
	uint16_t bufferSize;
	uint16_t*buffer;
};


uint16_t worldMapTemp[24][24]=
{
	{8,8,8,8,8,8,8,8,8,8,8,4,4,6,4,4,6,4,6,4,4,4,6,4},
	{8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
	{8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,6},
	{8,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
	{8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
	{8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,6,6,6,0,6,4,6},
	{8,8,8,8,0,8,8,8,8,8,8,4,4,4,4,4,4,6,0,0,0,0,0,6},
	{7,7,7,7,0,7,7,7,7,0,8,0,8,0,8,0,8,4,0,4,0,6,0,6},
	{7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,0,0,0,0,0,6},
	{7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,0,0,0,0,4},
	{7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,6,0,6,0,6},
	{7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,4,6,0,6,6,6},
	{7,7,7,7,0,7,7,7,7,8,8,4,0,6,8,4,8,3,3,3,0,3,3,3},
	{2,2,2,2,0,2,2,2,2,4,6,4,0,0,6,0,6,3,0,0,0,0,0,3},
	{2,2,0,0,0,0,0,2,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
	{2,0,0,0,0,0,0,0,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
	{1,0,0,0,0,0,0,0,1,4,4,4,4,4,6,0,6,3,3,0,0,0,3,3},
	{2,0,0,0,0,0,0,0,2,2,2,1,2,2,2,6,6,0,0,5,0,5,0,5},
	{2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
	{2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
	{2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
	{2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
	{2,2,2,2,1,2,2,2,2,2,2,1,2,2,2,5,5,5,5,5,5,5,5,5}
};

MapContext*OpenMap(const char*filename) {

	MapContext*mapContext = (MapContext*)malloc(sizeof(MapContext));
	if (mapContext == NULL) {
		return NULL;
	}
	mapContext->filename = strdup(filename);

	//### HARD CODED

	mapContext->width = 24;
	mapContext->height = 24;
	mapContext->bufferSize = mapContext->width*mapContext->height*sizeof(uint16_t);
	mapContext->buffer = (uint16_t*)malloc(mapContext->bufferSize);
	if (mapContext == NULL) {
		free(mapContext->filename);
		free(mapContext);
		return NULL;
	}
	memcpy(mapContext->buffer, worldMapTemp, mapContext->bufferSize);
    // Load textures
	OpenTexture("resource/image/barrel.png");
	OpenTexture("resource/image/eagle.png");
	OpenTexture("resource/image/redbrick.png");
	OpenTexture("resource/image/purplestone.png");
	OpenTexture("resource/image/greystone.png");
	OpenTexture("resource/image/bluestone.png");
	OpenTexture("resource/image/mossy.png");
	OpenTexture("resource/image/wood.png");
	OpenTexture("resource/image/colorstone.png");

	//###

	return mapContext;
}

uint16_t GetMapWidth(MapContext*mapContext) {
	return mapContext->width;
}

uint16_t GetMapHeight(MapContext*mapContext) {
	return mapContext->height;
}

uint16_t GetMapValue(MapContext*mapContext, uint16_t x, uint16_t y) {
	uint16_t*value = mapContext->buffer+(x*mapContext->width)+y;
	return *value;
}

void CloseMap(MapContext*mapContext) {
	free(mapContext->filename);
	free(mapContext->buffer);
	free(mapContext);
}




