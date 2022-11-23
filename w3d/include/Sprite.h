#ifndef W3D_SPRITE_H_
#define W3D_SPRITE_H_

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Defines.h"
#include "Texture.h"

typedef struct w3d_SpriteSt w3d_Sprite;

typedef enum {
    W3D_SPRITE_ANGLE_0 = 0,
    W3D_SPRITE_ANGLE_45 = 45,
    W3D_SPRITE_ANGLE_90 = 90,
    W3D_SPRITE_ANGLE_135 = 135,
    W3D_SPRITE_ANGLE_180 = 180,
    W3D_SPRITE_ANGLE_225 = 225,
    W3D_SPRITE_ANGLE_270 = 270,
    W3D_SPRITE_ANGLE_315 = 315
} w3d_SpriteDirection;

w3d_Sprite *w3d_Sprite_CreateFromRaw(
    const char *name,
    int32_t positionX,
    int32_t positionY,
    w3d_SpriteDirection directionAngle,
    bool visible,
    w3d_Texture *texture);

void w3d_Sprite_Destroy(w3d_Sprite *sprite);

int32_t w3d_Sprite_GetPositionX(w3d_Sprite *sprite);

int32_t w3d_Sprite_GetPositionY(w3d_Sprite *sprite);

w3d_Texture *w3d_Sprite_GetTexture(w3d_Sprite *sprite);

// Sprite animation

// int16_t w3d_Sprite_CreateAnimmation(
//     w3d_Sprite* sprite,
// 	uint16_t start,
// 	uint16_t size,
// 	uint16_t frameRate,
// 	bool loopActive);

// void w3d_Sprite_DestroyAnimation(int16_t animIndex);

// void w3d_Sprite_PlayAnimation(int16_t animIndex);

// void w3d_Sprite_PauseAnimation(int16_t animIndex);

// void w3d_Sprite_StopAnimation(int16_t animIndex);

// void w3d_Sprite_SetAnimationLoop(int16_t animIndex, bool loopActive);

#endif  // W3D_SPRITE_H_
