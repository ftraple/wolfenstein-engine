
#include "Camera.h"
#include "Defines.h"
#include "Texture.h"

typedef struct FrameData {
    int32_t mapX;
    int32_t mapY;

    int32_t wallStart;
    int32_t wallEnd;

    uint8_t side;  // Was a NS or a EW wall hit?
    float wallHitX;
    float perpWallDist;

    float rayDirectionX;
    float rayDirectionY;
} FrameData;

struct w3d_CameraSt {
    float positionX;
    float positionY;
    float directionX;
    float directionY;
    float planeX;
    float planeY;
    int width;
    int height;
    float runSpeed;
    float walkSpeed;
    float moveSpeed;
    float rotationSpeed;
    float wallDistance;
    w3d_Map* map;
    w3d_SpriteList* spriteList;
    uint32_t* viewBuffer;
    size_t viewBufferSize;

    double zBuffer[2048];

    FrameData* frameData;
};

void FloorRender();
void SpriteRender();

/*################################################################################*/
w3d_Camera* w3d_Camera_Create(int width, int height) {
    w3d_Camera* camera = (w3d_Camera*)malloc(sizeof(w3d_Camera));
    if (camera == NULL) {
        printf("Allocate camera context memory fail!\n");
        return NULL;
    }
    camera->positionX = 5;
    camera->positionY = 5;
    camera->directionX = -1;
    camera->directionY = 0;
    camera->planeX = 0;
    camera->planeY = 0.60;
    camera->width = width;
    camera->height = height;
    camera->runSpeed = 0.1;
    camera->walkSpeed = 0.05;
    camera->moveSpeed = camera->walkSpeed;
    camera->rotationSpeed = 0.001;
    camera->wallDistance = 0.3;
    camera->viewBufferSize = width * height * sizeof(uint32_t);
    camera->map = NULL;
    camera->spriteList = NULL;
    camera->viewBuffer = NULL;
    camera->frameData = (FrameData*)malloc(camera->width * sizeof(FrameData));
    if (camera->frameData == NULL) {
        printf("Allocate camera frame data buffer fail!\n");
        free(camera);
        return NULL;
    }

    return camera;
}

/*################################################################################*/
void w3d_Camera_Destroy(w3d_Camera* camera) {
    assert(camera != NULL);
    free(camera->frameData);
    free(camera);
}

/*################################################################################*/
void w3d_Camera_SetMap(w3d_Camera* camera, w3d_Map* map) {
    assert(camera != NULL);
    camera->map = map;
}

/*################################################################################*/
void w3d_Camera_SetSpriteList(w3d_Camera* camera, w3d_SpriteList* spriteList) {
    assert(camera != NULL);
    camera->spriteList = spriteList;
}

/*################################################################################*/
void w3d_Camera_Render(w3d_Camera* camera, void* viewBuffer) {
    assert(camera != NULL);
    if (camera->map == NULL) {
        return;
    }
    camera->viewBuffer = (uint32_t*)viewBuffer;
    memset(camera->viewBuffer, 128, camera->viewBufferSize);

    float cameraX;
    float rayPositionX = camera->positionX;
    float rayPositionY = camera->positionY;

    // Length of ray from current position to next x or y-side
    float sideDistanceX;
    float sideDistanceY;

    // what direction to step in x or y-direction (either +1 or -1)
    int stepX;
    int stepY;

    uint32_t texturePositionX;
    uint32_t texturePositionY;
    uint32_t* texturePixel;

    FrameData* frameData = camera->frameData;

    // Draw
    for (int x = 0; x < camera->width; x++) {
        // Step - Calculate ray position and direction
        cameraX = (2 * x / (float)camera->width) - 1;
        frameData->rayDirectionX = camera->directionX + (camera->planeX * cameraX);
        frameData->rayDirectionY = camera->directionY + (camera->planeY * cameraX);

        // which box of the map we're in
        frameData->mapX = (int32_t)rayPositionX;
        frameData->mapY = (int32_t)rayPositionY;

        //---> Step - Calculate the delta distance X and Y.
        float deltaDistX = sqrt(1 + (frameData->rayDirectionY * frameData->rayDirectionY) / (frameData->rayDirectionX * frameData->rayDirectionX));
        float deltaDistY = sqrt(1 + (frameData->rayDirectionX * frameData->rayDirectionX) / (frameData->rayDirectionY * frameData->rayDirectionY));

        //---> Step 2 - Calculate the initial step and the sideDistance.
        if (frameData->rayDirectionX < 0) {
            stepX = -1;
            sideDistanceX = (rayPositionX - frameData->mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistanceX = (frameData->mapX + 1.0 - rayPositionX) * deltaDistX;
        }
        if (frameData->rayDirectionY < 0) {
            stepY = -1;
            sideDistanceY = (rayPositionY - frameData->mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistanceY = (frameData->mapY + 1.0 - rayPositionY) * deltaDistY;
        }

        //---> Step 3 - Find a wall.
        bool hitWall = false;
        while (hitWall == false) {
            // jump to next map square, OR in x-direction, OR in y-direction
            if (sideDistanceX < sideDistanceY) {
                sideDistanceX += deltaDistX;
                frameData->mapX += stepX;
                frameData->side = 0;
            } else {
                sideDistanceY += deltaDistY;
                frameData->mapY += stepY;
                frameData->side = 1;
            }
            // Check if ray has hit a wall
            if (w3d_Map_GetValue(camera->map, frameData->mapX, frameData->mapY) > 0) {
                hitWall = true;
            }
        }

        // Calculate distance projected on camera direction (oblique distance will give fish eye effect!)
        if (frameData->side == 0) {
            frameData->perpWallDist = (frameData->mapX - rayPositionX + (1 - stepX) / 2) / frameData->rayDirectionX;
        } else {
            frameData->perpWallDist = (frameData->mapY - rayPositionY + (1 - stepY) / 2) / frameData->rayDirectionY;
        }

        // Get texture surface
        w3d_Texture* texture = w3d_Map_GetWallTexture(camera->map, frameData->mapX, frameData->mapY);
        if (texture == NULL) {
            continue;
        }

        // Calculate height of line to draw on screen
        int lineHeight = camera->height / frameData->perpWallDist;

        // calculate lowest and highest pixel to fill in current stripe
        frameData->wallStart = (-lineHeight / 2) + (camera->height / 2);
        if (frameData->wallStart < 0) {
            frameData->wallStart = 0;
        }
        frameData->wallEnd = (lineHeight / 2) + (camera->height / 2);
        if (frameData->wallEnd > camera->height) {
            frameData->wallEnd = camera->height;
        }

        // Calculate where exactly the wall was hit.
        if (frameData->side == 0) {
            frameData->wallHitX = rayPositionY + frameData->perpWallDist * frameData->rayDirectionY;
        } else {
            frameData->wallHitX = rayPositionX + frameData->perpWallDist * frameData->rayDirectionX;
        }
        frameData->wallHitX -= floor(frameData->wallHitX);

        // Calculate X coordinate on the texture
        texturePositionX = (int)(frameData->wallHitX * (double)texture->width);
        if (frameData->side == 0 && frameData->rayDirectionX > 0) {
            texturePositionX = texture->width - texturePositionX - 1;
        }
        if (frameData->side == 1 && frameData->rayDirectionY < 0) {
            texturePositionX = texture->width - texturePositionX - 1;
        }

        // Draw the wall line
        for (int y = frameData->wallStart; y < frameData->wallEnd; ++y) {
            // 256 and 128 factors to avoid floats
            uint32_t d = (y * 256) - (camera->height * 128) + (lineHeight * 128);
            texturePositionY = ((d * texture->height) / lineHeight) / 256;
            // Get the pixel color in the texture buffer
            uint32_t textureBufferOffset = (texture->height * texturePositionY) + texturePositionX;
            texturePixel = (uint32_t*)texture->pixelData + textureBufferOffset;
            // Set the pixel color in the camera view buffer
            uint32_t viewBufferOffset = (camera->width * y) + x;
            camera->viewBuffer[viewBufferOffset] = *texturePixel;
        }

        // SET THE ZBUFFER FOR THE SPRITE CASTING
        camera->zBuffer[x] = frameData->perpWallDist;  // perpendicular distance is used

        frameData++;
    }

    FloorRender(camera);
    SpriteRender(camera);
}

/*################################################################################*/
void FloorRender(w3d_Camera* camera) {
    FrameData* frameData = camera->frameData;

    for (int x = 0; x < camera->width; x++) {
        // FLOOR CASTING -------------------------------------------------------------
        double floorXWall, floorYWall;  // x, y position of the floor texel at the bottom of the wall

        // 4 different wall directions possible
        if (frameData->side == 0 && frameData->rayDirectionX > 0) {
            floorXWall = frameData->mapX;
            floorYWall = frameData->mapY + frameData->wallHitX;
        } else if (frameData->side == 0 && frameData->rayDirectionX < 0) {
            floorXWall = frameData->mapX + 1.0;
            floorYWall = frameData->mapY + frameData->wallHitX;
        } else if (frameData->side == 1 && frameData->rayDirectionY > 0) {
            floorXWall = frameData->mapX + frameData->wallHitX;
            floorYWall = frameData->mapY;
        } else {
            floorXWall = frameData->mapX + frameData->wallHitX;
            floorYWall = frameData->mapY + 1.0;
        }

        double distWall, distPlayer, currentDist;

        distWall = frameData->perpWallDist;
        distPlayer = 0.0;

        if (frameData->wallEnd < 0) {
            frameData->wallEnd = camera->height;  // becomes < 0 when the integer overflows
        }

        // draw the floor from wallEnd to the bottom of the screen
        for (int y = frameData->wallEnd + 1; y < camera->height; y++) {
            currentDist = camera->height / (2.0 * y - camera->height);  // you could make a small lookup table for this instead

            double weight = (currentDist - distPlayer) / (distWall - distPlayer);

            double currentFloorX = weight * floorXWall + (1.0 - weight) * camera->positionX;
            double currentFloorY = weight * floorYWall + (1.0 - weight) * camera->positionY;

            w3d_Texture* floorTexture = w3d_Map_GetFloorTexture(camera->map, (uint16_t)currentFloorX, (uint16_t)currentFloorY);
            w3d_Texture* ceilingTexture = w3d_Map_GetCeilingTexture(camera->map, (uint16_t)currentFloorX, (uint16_t)currentFloorY);

            int floorTexX;
            int floorTexY;

            floorTexX = (int)(currentFloorX * floorTexture->width) % floorTexture->width;
            floorTexY = (int)(currentFloorY * floorTexture->height) % floorTexture->height;

            // Floor
            uint32_t* pixel = (uint32_t*)floorTexture->pixelData + (floorTexture->width * floorTexY) + floorTexX;
            camera->viewBuffer[(camera->width * y) + x] = *pixel;
            // Ceiling
            pixel = (uint32_t*)ceilingTexture->pixelData + (ceilingTexture->width * floorTexY) + floorTexX;
            camera->viewBuffer[(camera->width * (camera->height - y)) + x] = *pixel;
        }
        frameData++;
    }
}

void SpriteRender(w3d_Camera* camera) {
    if (camera->spriteList == NULL) {
        return;
    }

    size_t spriteAmount = w3d_SpriteList_GetSize(camera->spriteList);

    w3d_SpriteList_SortFromFarToClose(camera->spriteList, camera->positionX, camera->positionY);

    // After sorting the sprites, do the projection and draw them
    for (size_t i = 0; i < spriteAmount; i++) {
        w3d_Sprite* sprite = w3d_SpriteList_GetSprite(camera->spriteList, i);
        w3d_Texture* texture = w3d_Sprite_GetTexture(sprite);

        // translate sprite position relative to camera
        float spriteX = w3d_Sprite_GetPositionX(sprite) - camera->positionX;
        float spriteY = w3d_Sprite_GetPositionY(sprite) - camera->positionY;

        // transform sprite with the inverse camera matrix
        //  [ planeX   dirX ] -1                                       [ dirY      -dirX ]
        //  [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
        //  [ planeY   dirY ]                                          [ -planeY  planeX ]

        float invDet = 1.0 / (camera->planeX * camera->directionY - camera->directionX * camera->planeY);  // required for correct matrix multiplication

        float transformX = invDet * (camera->directionY * spriteX - camera->directionX * spriteY);
        float transformY = invDet * (-camera->planeY * spriteX + camera->planeX * spriteY);  // this is actually the depth inside the screen, that what Z is in 3D

        int spriteScreenX = (int)((camera->width / 2) * (1 + transformX / transformY));

        // calculate height of the sprite on screen
        int spriteHeight = abs((int)(camera->height / (transformY)));  // using "transformY" instead of the real distance prevents fisheye
        // calculate lowest and highest pixel to fill in current stripe
        int wallStartY = -spriteHeight / 2 + camera->height / 2;
        if (wallStartY < 0) wallStartY = 0;
        int wallEndY = spriteHeight / 2 + camera->height / 2;
        if (wallEndY >= camera->height) wallEndY = camera->height - 1;

        // calculate width of the sprite
        int spriteWidth = abs((int)(camera->height / (transformY)));
        int wallStartX = -spriteWidth / 2 + spriteScreenX;
        if (wallStartX < 0) wallStartX = 0;
        int wallEndX = spriteWidth / 2 + spriteScreenX;
        if (wallEndX >= camera->width) wallEndX = camera->width - 1;

        // loop through every vertical stripe of the sprite on screen
        for (int stripe = wallStartX; stripe < wallEndX; stripe++) {
            int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texture->width / spriteWidth) / 256;
            // the conditions in the if are:
            // 1) it's in front of camera plane so you don't see things behind you
            // 2) it's on the screen (left)
            // 3) it's on the screen (right)
            // 4) ZBuffer, with perpendicular distance

            if (transformY > 0 && stripe > 0 && stripe < camera->width && transformY < camera->zBuffer[stripe]) {
                for (int y = wallStartY; y < wallEndY; y++) {
                    int d = (y)*256 - camera->height * 128 + spriteHeight * 128;  // 256 and 128 factors to avoid floats
                    int texY = ((d * texture->height) / spriteHeight) / 256;

                    uint32_t* pixelColor = (uint32_t*)texture->pixelData + (texture->width * texY) + texX;
                    if ((*pixelColor & 0x00FFFFFF) != 0) {  // paint pixel if it isn't black, black is the invisible color
                        camera->viewBuffer[(camera->width * y) + stripe] = *pixelColor;
                    }
                }
            }
        }
    }
}

/*################################################################################*/
void w3d_Camera_Rotate(w3d_Camera* camera, int32_t stepX) {
    float rotSpeed = (float)stepX * camera->rotationSpeed;
    float oldDirX = camera->directionX;
    float oldPlaneX = camera->planeX;
    // Both camera direction and camera plane must be rotated
    camera->directionX = camera->directionX * cos(rotSpeed) - camera->directionY * sin(rotSpeed);
    camera->directionY = oldDirX * sin(rotSpeed) + camera->directionY * cos(rotSpeed);
    camera->planeX = camera->planeX * cos(rotSpeed) - camera->planeY * sin(rotSpeed);
    camera->planeY = oldPlaneX * sin(rotSpeed) + camera->planeY * cos(rotSpeed);
}

/*################################################################################*/
void w3d_Camera_MoveForward(w3d_Camera* camera) {
    if (w3d_Map_GetValue(camera->map, (int)(camera->positionX + camera->directionX * (camera->moveSpeed + camera->wallDistance)), (int)camera->positionY) == 0) {
        camera->positionX += camera->directionX * camera->moveSpeed;
    }
    if (w3d_Map_GetValue(camera->map, (int)camera->positionX, (int)(camera->positionY + camera->directionY * (camera->moveSpeed + camera->wallDistance))) == 0) {
        camera->positionY += camera->directionY * camera->moveSpeed;
    }
}

/*################################################################################*/
void w3d_Camera_MoveBackward(w3d_Camera* camera) {
    if (w3d_Map_GetValue(camera->map, (int)(camera->positionX - camera->directionX * (camera->moveSpeed + camera->wallDistance)), (int)camera->positionY) == 0) {
        camera->positionX -= camera->directionX * camera->moveSpeed;
    }
    if (w3d_Map_GetValue(camera->map, (int)camera->positionX, (int)(camera->positionY - camera->directionY * (camera->moveSpeed + camera->wallDistance))) == 0) {
        camera->positionY -= camera->directionY * camera->moveSpeed;
    }
}

/*################################################################################*/
void w3d_Camera_MoveLeft(w3d_Camera* camera) {
    float rotation = 80.0;
    float directionX = camera->directionX * cos(rotation) - camera->directionY * sin(rotation);
    float directionY = camera->directionX * sin(rotation) + camera->directionY * cos(rotation);

    if (w3d_Map_GetValue(camera->map, (int)(camera->positionX - directionX * (camera->moveSpeed + camera->wallDistance)), (int)camera->positionY) == 0) {
        camera->positionX -= directionX * camera->moveSpeed;
    }
    if (w3d_Map_GetValue(camera->map, (int)camera->positionX, (int)(camera->positionY - directionY * (camera->moveSpeed + camera->wallDistance))) == 0) {
        camera->positionY -= directionY * camera->moveSpeed;
    }
}

/*################################################################################*/
void w3d_Camera_MoveRight(w3d_Camera* camera) {
    double rotation = 80.0;
    float directionX = camera->directionX * cos(rotation) - camera->directionY * sin(rotation);
    float directionY = camera->directionX * sin(rotation) + camera->directionY * cos(rotation);

    if (w3d_Map_GetValue(camera->map, (int)(camera->positionX + directionX * (camera->moveSpeed + camera->wallDistance)), (int)camera->positionY) == 0) {
        camera->positionX += directionX * camera->moveSpeed;
    }
    if (w3d_Map_GetValue(camera->map, (int)camera->positionX, (int)(camera->positionY + directionY * (camera->moveSpeed + camera->wallDistance))) == 0) {
        camera->positionY += directionY * camera->moveSpeed;
    }
}
