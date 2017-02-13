/*
 * Texture.h
 *
 *  Created on: 3 de fev de 2017
 *      Author: fabiano
 */

#ifndef INCLUDE_TEXTURE_H_
#define INCLUDE_TEXTURE_H_

#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Return the texture index
uint16_t OpenTexture(const char*filename);

SDL_Surface*GetTextureSurface(uint16_t index);

void CloseTexture(uint16_t index);

#endif /* INCLUDE_TEXTURE_H_ */
