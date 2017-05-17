#ifndef INCLUDE_SPRITE_H_
#define INCLUDE_SPRITE_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct Sprite Sprite;

typedef enum {
	ANGLE_0 = 0,
	ANGLE_45 = 45,
	ANGLE_90 = 90,
	ANGLE_135 = 135,
	ANGLE_180 = 180,
	ANGLE_225 = 225,
	ANGLE_270 = 270,
	ANGLE_315 = 315
} SpriteDirection;

Sprite*CreateSprite( const char*textureFilename,
					 uint16_t positionX,
					 uint16_t positionY,
					 uint16_t width,
					 uint16_t height,
					 SpriteDirection directionAngle,
					 bool visible);

void DestroySprite();

void SetSpriteVisible(Sprite*sprite, bool visible);

void SetSpritePosition(Sprite*sprite, uint16_t positionX, uint16_t positionY);

uint16_t GetSpritePositionX(Sprite*sprite);

uint16_t GetSpritePositionY(Sprite*sprite);

void SetSpriteDirectionAngle(Sprite*sprite, SpriteDirection directionAngle);

// Sprite animation

int16_t CreateSpriteAnim(Sprite*sprite,
						 uint16_t start,
						 uint16_t size,
						 uint16_t frameRate,
						 bool loopActive);

void DestroySpriteAnim(int16_t animIndex);

void PlaySpriteAnim(int16_t animIndex);

void PauseSpriteAnim(int16_t animIndex);

void StopSpriteAnim(int16_t animIndex);

void SetSpriteAnimLoop(int16_t animIndex, bool loopActive);



#endif /* INCLUDE_SPRITE_H_ */
