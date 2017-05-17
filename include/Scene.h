#ifndef INCLUDE_SCENE_H_
#define INCLUDE_SCENE_H_

#include "Map.h"
#include "Camera.h"
#include "Sprite.h"

typedef struct Scene Scene;

Scene*Scene_Create(Map*map);

void Scene_Destroy(Scene*scene);

void Scene_AddCamera(Scene*scene, Camera*camera);

void Scene_RemoveCamera(Scene*scene, Camera*camera);

void Scene_AddSprite(Scene*scene, Sprite*sprite);

void Scene_RemoveSprite(Scene*scene, Sprite*sprite);

void Scene_Update(Scene*scene);

void Scene_Render(Scene*scene);

#endif /* INCLUDE_SCENE_H_ */
