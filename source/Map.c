
#include "Defs.h"
#include "Map.h"

struct w3d_MapSt {
	int width;
	int height;
	uint8_t* data;
	size_t dataSize;
	w3d_ColorARGB floorColor;
	w3d_ColorARGB ceilingColor;
};

/*################################################################################*/
w3d_Map* w3d_CreateMapFromRaw(int width, int height, uint8_t* data) {
	assert(width > 1);
	assert(height > 1);
	assert(data != NULL);

	w3d_Map* map = (w3d_Map*)malloc(sizeof(w3d_Map));
	if (map == NULL) {
		return NULL;
	}
	map->width = width;
	map->height = height;
	map->dataSize = width * height * sizeof(uint8_t);
	map->data = (uint8_t*)malloc(map->dataSize);
	if (map->data == NULL) {
		free(map);
		return NULL;
	}
	memcpy(map->data, data, map->dataSize);

	return map;
}

/*################################################################################*/
void w3d_CloseMap(w3d_Map* map) {
	assert(map != NULL);
	free(map->data);
	free(map);
}

/*################################################################################*/
int w3d_GetMapWidth(w3d_Map* map) {
	assert(map != NULL);
	return map->width;
}

/*################################################################################*/
int w3d_GetMapHeight(w3d_Map* map) {
	assert(map != NULL);
	return map->height;
}

/*################################################################################*/
uint8_t w3d_GetMapValue(w3d_Map* map, int x, int y) {
	assert(map != NULL);
	uint8_t* value = map->data+((x*map->width)+y); 
	return *value;
}

