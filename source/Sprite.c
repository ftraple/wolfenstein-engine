
#include "Sprite.h"

typedef enum {
	W3D_SPRITE_ANIMATION_STATE_STOP = 0,
	W3D_SPRITE_ANIMATION_STATE_PLAY,
	W3D_SPRITE_ANIMATION_STATE_PAUSE
} w3d_SpriteAnimationState;

typedef struct {
	bool inUse;
	w3d_Sprite* sprite;
	uint16_t start;
	uint16_t size;
	uint16_t frameRate;
	bool loopActive;
} w3d_SpriteAnimation;

struct w3d_SpriteSt {
    char* name;
	int32_t positionX;
	int32_t positionY;
	uint16_t directionAngle;
	bool visible;
	w3d_Texture* texture;

	//w3d_ColorARGB invisibleColor;
	//bool colideActive;
	//uint16_t colideRadius;

	//w3d_SpriteAnimationState animationState;
	//int16_t animationIndex;
};


/*################################################################################*/
w3d_Sprite* w3d_Sprite_CreateFromRaw(
    const char* name,
	int32_t positionX,
	int32_t positionY,
	w3d_SpriteDirection directionAngle,
	bool visible, 
	w3d_Texture* texture) {

	w3d_Sprite* sprite = (w3d_Sprite*)malloc(sizeof(w3d_Sprite));
	if (sprite == NULL) {
		return NULL;
	}
	sprite->name = strdup(name);
	sprite->positionX = positionX;
	sprite->positionY = positionY;
	sprite->directionAngle = directionAngle;
	sprite->visible = visible;
	sprite->texture = texture;

	//sprite->animationIndex = -1;
	//sprite->animationState = W3D_SPRITE_ANIMATION_STATE_STOP;

	return sprite;
}

/*################################################################################*/
void w3d_Sprite_Destroy(w3d_Sprite* sprite) {
    assert(sprite != NULL);
	free(sprite);
}

/*################################################################################*/
int32_t w3d_Sprite_GetPositionX(w3d_Sprite* sprite) {
    assert(sprite != NULL);
	return sprite->positionX;
}

/*################################################################################*/
int32_t w3d_Sprite_GetPositionY(w3d_Sprite* sprite) {
    assert(sprite != NULL);
	return sprite->positionY;
}

/*################################################################################*/
w3d_Texture* w3d_Sprite_GetTexture(w3d_Sprite* sprite) {
    assert(sprite != NULL);
	return sprite->texture;
}

// // Animation

// int16_t CreateSpriteAnim(Sprite* sprite,
// 					  	  uint16_t start,
// 						  uint16_t size,
// 						  uint16_t frameRate,
// 						  bool loopActive) {

// 	int16_t index = 0;

// 	// Allocate sprite animation list memory if needed.
// 	if (pvSpriteAnimList == NULL) {
// 		pvSpriteAnimListSize = 100;
// 		pvSpriteAnimList = (SpriteAnim*)malloc(pvSpriteAnimListSize*sizeof(SpriteAnim));
// 		if (pvSpriteAnimList == NULL) {
// 			printf( "Allocate memory fail!\n");
// 			return -1;
// 		}
// 		memset(pvSpriteAnimList, 0, pvSpriteAnimListSize*sizeof(SpriteAnim));
// 	}
// 	else if (pvSpriteAnimListInUse == pvSpriteAnimListSize) {
// 		pvSpriteAnimListSize += 100;
// 		SpriteAnim*spriteAnimListTemp = (SpriteAnim*)realloc(pvSpriteAnimList,pvSpriteAnimListSize*sizeof(SpriteAnim));
// 		if (spriteAnimListTemp == NULL) {
// 			printf( "Allocate memory fail!\n");
// 			return -1;
// 		}
// 		pvSpriteAnimList = spriteAnimListTemp;
// 	}

// 	// Find empty structure in the list.
// 	while (pvSpriteAnimList[index].inUse == true) {
// 		index++;
// 	}

// 	pvSpriteAnimList[index].inUse = true;
// 	pvSpriteAnimList[index].sprite = sprite;
// 	pvSpriteAnimList[index].start = start;
// 	pvSpriteAnimList[index].size = size;
// 	pvSpriteAnimList[index].frameRate = frameRate;
// 	pvSpriteAnimList[index].loopActive= loopActive;

// 	return index;
// }

// void DestroySpriteAnim(int16_t animIndex) {
// 	if (pvSpriteAnimList[animIndex].inUse == true) {
// 		pvSpriteAnimList[animIndex].inUse = false;
// 	}
// }

// void PlaySpriteAnim(int16_t animIndex) {
// 	if (pvSpriteAnimList[animIndex].inUse == false) {
// 		return;
// 	}
// }

// void PauseSpriteAnim(int16_t animIndex) {
// 	if (pvSpriteAnimList[animIndex].inUse == false) {
// 		return;
// 	}

// }

// void StopSpriteAnim(int16_t animIndex) {
// 	if (pvSpriteAnimList[animIndex].inUse == false) {
// 		return;
// 	}
// }

// void SetSpriteAnimLoop(int16_t animIndex, bool loopActive) {
// 	if (pvSpriteAnimList[animIndex].inUse == false) {
// 		return;
// 	}
// 	pvSpriteAnimList[animIndex].loopActive = loopActive;

// }

