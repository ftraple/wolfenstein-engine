#ifndef W3D_DEFINES_H_
#define W3D_DEFINES_H_

#include <stdint.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_FPS 60

typedef struct {
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} w3d_ColorARGB;

#endif  // W3D_DEFINES_H_
