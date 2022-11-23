
#include "Map.h"
#include "Defines.h"

struct w3d_MapSt {
    int width;
    int height;
    uint8_t* dataFloor;
    uint8_t* dataWall;
    uint8_t* dataCeiling;
    size_t dataSize;
    w3d_TextureList* textureList;
    w3d_ColorARGB floorColor;
    w3d_ColorARGB ceilingColor;
};

/*################################################################################*/
w3d_Map* w3d_Map_CreateFromRaw(int width, int height, uint8_t* dataFloor, uint8_t* dataWall, uint8_t* dataCeiling, w3d_TextureList* textureList) {
    assert(width > 1);
    assert(height > 1);
    assert(dataFloor != NULL);
    assert(dataWall != NULL);
    assert(dataCeiling != NULL);

    w3d_Map* map = (w3d_Map*)malloc(sizeof(w3d_Map));
    if (map == NULL) {
        return NULL;
    }
    map->width = width;
    map->height = height;
    map->textureList = textureList;
    map->dataSize = width * height * sizeof(uint8_t);
    map->dataFloor = (uint8_t*)malloc(map->dataSize);
    map->dataWall = (uint8_t*)malloc(map->dataSize);
    map->dataCeiling = (uint8_t*)malloc(map->dataSize);
    memcpy(map->dataFloor, dataFloor, map->dataSize);
    memcpy(map->dataWall, dataWall, map->dataSize);
    memcpy(map->dataCeiling, dataCeiling, map->dataSize);

    return map;
}

/*################################################################################*/
void w3d_Map_Close(w3d_Map* map) {
    assert(map != NULL);
    free(map->dataFloor);
    free(map->dataWall);
    free(map->dataCeiling);
    free(map);
}

/*################################################################################*/
int w3d_Map_GetWidth(w3d_Map* map) {
    assert(map != NULL);
    return map->width;
}

/*################################################################################*/
int w3d_Map_GetHeight(w3d_Map* map) {
    assert(map != NULL);
    return map->height;
}

/*################################################################################*/
uint8_t w3d_Map_GetValue(w3d_Map* map, int x, int y) {
    assert(map != NULL);
    uint8_t* value = map->dataWall + ((x * map->width) + y);
    return *value;
}

/*################################################################################*/
w3d_Texture* w3d_Map_GetFloorTexture(w3d_Map* map, int x, int y) {
    assert(map != NULL);
    uint8_t* value = map->dataFloor + ((x * map->width) + y);
    return w3d_TextureList_GetByIndex(map->textureList, *value);  // TODO hardcoded
}

/*################################################################################*/
w3d_Texture* w3d_Map_GetWallTexture(w3d_Map* map, int x, int y) {
    assert(map != NULL);
    uint8_t* value = map->dataWall + ((x * map->width) + y);
    return w3d_TextureList_GetByIndex(map->textureList, *value);
}

/*################################################################################*/
w3d_Texture* w3d_Map_GetCeilingTexture(w3d_Map* map, int x, int y) {
    assert(map != NULL);
    uint8_t* value = map->dataCeiling + ((x * map->width) + y);
    return w3d_TextureList_GetByIndex(map->textureList, *value);  // TODO hardcoded
}
