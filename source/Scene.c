
#include <stdio.h>
#include <stdlib.h>

#include "Scene.h"

typedef struct CameraElement CameraElement;
typedef struct SpriteElement SpriteElement;

struct CameraElement {
	Camera*camera;
	CameraElement*next;
};

struct SpriteElement {
	Sprite*sprite;
	SpriteElement*next;
};

struct Scene {
	char*name;
	Map*map;
	size_t cameraListSize;
	CameraElement*cameraList;
	size_t spriteListSize;
	SpriteElement*spriteList;
};

Scene*Scene_Create(Map*map) {

	Scene*scene = (Scene*)malloc(sizeof(Scene));
	if (scene == NULL) {
		return NULL;
	}

	scene->map = map;

	scene->cameraListSize = 0;
	scene->cameraList = NULL;

	scene->spriteListSize = 0;
	scene->spriteList = NULL;

	return scene;
}

void Scene_Destroy(Scene*scene) {

	if (scene == NULL) {
		return;
	}
	free(scene);
}

void Scene_AddCamera(Scene*scene, Camera*camera) {
	// Create a new camera element
	CameraElement*cameraElement = (CameraElement*)malloc(sizeof(CameraElement));
	if (cameraElement == NULL) {
		return;
	}
	cameraElement->camera = camera;
	// Add new camera element to the list begin
	cameraElement->next = scene->cameraList;
	scene->cameraList = cameraElement;
	scene->cameraListSize++;
	// Set camera map
	SetCameraMap(camera, scene->map);
}


void Scene_RemoveCamera(Scene*scene, Camera*camera) {
	// Check if is the first element
	if (scene->cameraList->camera == camera) {
		scene->cameraList = scene->cameraList->next;
		scene->cameraListSize--;
		free(camera);
		return;
	}
	// Find the element in the list
	CameraElement*cameraPrevious = scene->cameraList;
	CameraElement*cameraIt = scene->cameraList->next;
	while (cameraIt != NULL) {
		if (cameraIt->camera == camera) {
			cameraPrevious->next = cameraIt->next;
			scene->cameraListSize--;
			free(camera);
			return;
		}
	}
}


void Scene_AddSprite(Scene*scene, Sprite*sprite) {
	// Create a new sprite element
	SpriteElement*spriteElement = (SpriteElement*)malloc(sizeof(SpriteElement));
	if (spriteElement == NULL) {
		return;
	}
	spriteElement->sprite = sprite;
	// Add new sprite element to the list begin
	spriteElement->next = scene->spriteList;
	scene->spriteList = spriteElement;
	scene->spriteListSize++;
}

void Scene_RemoveSprite(Scene*scene, Sprite*sprite) {
	// Check if is the first element
	if (scene->spriteList->sprite == sprite) {
		scene->spriteList = scene->spriteList->next;
		scene->spriteListSize--;
		free(sprite);
		return;
	}
	// Find the element in the list
	SpriteElement*spritePrevious = scene->spriteList;
	SpriteElement*spriteIt = scene->spriteList->next;
	while (spriteIt != NULL) {
		if (spriteIt->sprite == sprite) {
			spritePrevious->next = spriteIt->next;
			scene->spriteListSize--;
			free(sprite);
			return;
		}
	}
}

void Scene_Update(Scene*scene) {
	if (scene == NULL) {
		return;
	}
}

void Scene_Render(Scene*scene) {

	// Render sound

	// Render all cameras
	CameraElement*cameraIt = scene->cameraList;
	while (cameraIt != NULL) {
		RenderCamera(cameraIt->camera);
		cameraIt = cameraIt->next;
	}
}


