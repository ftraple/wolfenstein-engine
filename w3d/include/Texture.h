#ifndef W3D_TEXTURE_H_
#define W3D_TEXTURE_H_

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct w3d_TextureSt w3d_Texture;
struct w3d_TextureSt {
    char* name;
    uint16_t width;
    uint16_t height;
    uint32_t* pixelData;
    size_t pixelDataSize;
};

w3d_Texture* w3d_Texture_CretateFromRaw(const char* name, uint16_t width, uint16_t height, uint32_t* pixelDataABGR);

void w3d_Texture_Destroy(w3d_Texture* texture);

#endif  // W3D_TEXTURE_H_
