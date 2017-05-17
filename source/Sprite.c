/*
 * Sprite.c
 *
 *  Created on: 24 de fev de 2017
 *      Author: fabiano
 */

#include <stdio.h>
#include <stdlib.h>

#include "Sprite.h"
#include "Texture.h"
#include "Defs.h"

typedef enum {
	SPRITE_ANIM_STATE_STOP = 0,
	SPRITE_ANIM_STATE_PLAY,
	SPRITE_ANIM_STATE_PAUSE
} SpriteAnimState;

// Sprite animation
typedef struct {
	bool inUse;
	Sprite*sprite;
	uint16_t start;
	uint16_t size;
	uint16_t frameRate;
	bool loopActive;
} SpriteAnim;

struct Sprite {
	int16_t textureIndex;
	uint16_t positionX;
	uint16_t positionY;
	uint16_t width;
	uint16_t height;
	uint16_t directionAngle;
	bool visible;

	color invisibleColor;

	bool colideActive;
	uint16_t colideRadius;

	SpriteAnimState animState;
	int16_t animIndex;

	struct Sprite*next;
};

Sprite*CreateSprite( const char*textureFilename,
					 uint16_t positionX,
					 uint16_t positionY,
					 uint16_t width,
					 uint16_t height,
					 SpriteDirection directionAngle,
					 bool visible) {

	Sprite*sprite = (Sprite*)malloc(sizeof(Sprite));
	if (sprite == NULL) {
		return NULL;
	}

	sprite->textureIndex = OpenTexture(textureFilename);
	if (sprite->textureIndex == 0) {
		free(sprite);
		return NULL;
	}
	sprite->positionX = positionX;
	sprite->positionY = positionY;
	sprite->width = width;
	sprite->height = height;
	sprite->directionAngle = directionAngle;
	sprite->visible = visible;

	// Animation state
	sprite->animIndex = -1;
	sprite->animState = SPRITE_ANIM_STATE_STOP;

	return sprite;
}

void DestroySprite(Sprite*sprite) {
	if (sprite != NULL) {
		free(sprite);
	}
}

void SetSpriteVisible(Sprite*sprite, bool visible) {
	sprite->visible = visible;
}

void SetSpritePosition(Sprite*sprite, uint16_t positionX, uint16_t positionY) {
	sprite->positionX = positionX;
	sprite->positionY = positionY;
}

uint16_t GetSpritePositionX(Sprite*sprite) {
	return sprite->positionX;
}

uint16_t GetSpritePositionY(Sprite*sprite) {
	return sprite->positionY;
}

void SetSpriteDirectionAngle(Sprite*sprite, SpriteDirection directionAngle) {
	sprite->directionAngle = directionAngle;
}

//// Animation
//
//int16_t CreateSpriteAnim(Sprite*sprite,
//					  	  uint16_t start,
//						  uint16_t size,
//						  uint16_t frameRate,
//						  bool loopActive) {
//
//	int16_t index = 0;
//
//	// Allocate sprite animation list memory if needed.
//	if (pvSpriteAnimList == NULL) {
//		pvSpriteAnimListSize = 100;
//		pvSpriteAnimList = (SpriteAnim*)malloc(pvSpriteAnimListSize*sizeof(SpriteAnim));
//		if (pvSpriteAnimList == NULL) {
//			printf( "Allocate memory fail!\n");
//			return -1;
//		}
//		memset(pvSpriteAnimList, 0, pvSpriteAnimListSize*sizeof(SpriteAnim));
//	}
//	else if (pvSpriteAnimListInUse == pvSpriteAnimListSize) {
//		pvSpriteAnimListSize += 100;
//		SpriteAnim*spriteAnimListTemp = (SpriteAnim*)realloc(pvSpriteAnimList,pvSpriteAnimListSize*sizeof(SpriteAnim));
//		if (spriteAnimListTemp == NULL) {
//			printf( "Allocate memory fail!\n");
//			return -1;
//		}
//		pvSpriteAnimList = spriteAnimListTemp;
//	}
//
//	// Find empty structure in the list.
//	while (pvSpriteAnimList[index].inUse == true) {
//		index++;
//	}
//
//	pvSpriteAnimList[index].inUse = true;
//	pvSpriteAnimList[index].sprite = sprite;
//	pvSpriteAnimList[index].start = start;
//	pvSpriteAnimList[index].size = size;
//	pvSpriteAnimList[index].frameRate = frameRate;
//	pvSpriteAnimList[index].loopActive= loopActive;
//
//	return index;
//}
//
//void DestroySpriteAnim(int16_t animIndex) {
//	if (pvSpriteAnimList[animIndex].inUse == true) {
//		pvSpriteAnimList[animIndex].inUse = false;
//	}
//}
//
//void PlaySpriteAnim(int16_t animIndex) {
//	if (pvSpriteAnimList[animIndex].inUse == false) {
//		return;
//	}
//}
//
//void PauseSpriteAnim(int16_t animIndex) {
//	if (pvSpriteAnimList[animIndex].inUse == false) {
//		return;
//	}
//
//}
//
//void StopSpriteAnim(int16_t animIndex) {
//	if (pvSpriteAnimList[animIndex].inUse == false) {
//		return;
//	}
//}
//
//void SetSpriteAnimLoop(int16_t animIndex, bool loopActive) {
//	if (pvSpriteAnimList[animIndex].inUse == false) {
//		return;
//	}
//	pvSpriteAnimList[animIndex].loopActive = loopActive;
//
//}

