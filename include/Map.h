/*
 * Map.h
 *
 *  Created on: 3 de fev de 2017
 *      Author: fabiano
 */

#ifndef INCLUDE_MAP_H_
#define INCLUDE_MAP_H_

#include <stdint.h>

struct MapContext;
typedef struct MapContext MapContext;

MapContext*OpenMap(const char*filename);

uint16_t GetMapWidth(MapContext*mapContext);

uint16_t GetMapHeight(MapContext*mapContext);

uint16_t GetMapValue(MapContext*mapContext, uint16_t x, uint16_t y);

void CloseMap(MapContext*mapContext);

#endif /* INCLUDE_MAP_H_ */
