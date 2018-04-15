
#include "SpriteList.h"

struct w3d_SpriteListSt {
    size_t size;
    w3d_Sprite** sprite;
};

/*################################################################################*/
w3d_SpriteList* w3d_SpriteList_Create(size_t size) {
	w3d_SpriteList* spriteList = (w3d_SpriteList*)malloc(sizeof(w3d_SpriteList));
	if (spriteList == NULL) {
		return NULL;
	}
	spriteList->size = 0;
	spriteList->sprite = (w3d_Sprite**)malloc(size*sizeof(w3d_Sprite*));
	if (spriteList->sprite == NULL) {
		free(spriteList);
		return NULL;
	}
	return spriteList;
}

/*################################################################################*/
void w3d_SpriteList_Destroy(w3d_SpriteList* spriteList) {
	assert(spriteList != NULL);
	free(spriteList->sprite);
	free(spriteList);
}

/*################################################################################*/
void w3d_SpriteList_Add(w3d_SpriteList* spriteList, w3d_Sprite* sprite) {
	assert(spriteList != NULL);
	spriteList->sprite[spriteList->size] = sprite;
	spriteList->size++;
}

/*################################################################################*/
size_t w3d_SpriteList_GetSize(w3d_SpriteList* spriteList) {
	assert(spriteList != NULL);
	return spriteList->size;
}

/*################################################################################*/
w3d_Sprite* w3d_SpriteList_GetSprite(w3d_SpriteList* spriteList, size_t index) {
	assert(spriteList != NULL);
	if (index >= spriteList->size) {
		return NULL;
	}
	return spriteList->sprite[index];
}

/*################################################################################*/
void w3d_SpriteList_SortFromFarToClose(w3d_SpriteList* spriteList, double positionX, double positionY) {
	assert(spriteList != NULL);

    // Calcluate sprite distances from a especific point
    double spriteDistance[spriteList->size];
	for(size_t i = 0; i < spriteList->size; i++) {
        double x = w3d_Sprite_GetPositionX(spriteList->sprite[i]);
        double y = w3d_Sprite_GetPositionY(spriteList->sprite[i]);
		spriteDistance[i] = ((positionX - x) * (positionX - x) + (positionY - y) * (positionY - y));
	}

    // Sort
	int gap = spriteList->size;
	bool swapped = false;
	while (gap > 1 || swapped) {
        // Shrink factor 1.3
		gap = (gap * 10) / 13;
		if (gap == 9 || gap == 10) {
			gap = 11;
		}
		if (gap < 1) {
			gap = 1;
		}
        // Swapp
		swapped = false;
		for (size_t i = 0; i < spriteList->size - gap; i++) {
			size_t j = i + gap;
			if (spriteDistance[i] < spriteDistance[j]) {
				// Swap distance
				double spriteDistanceTemp = spriteDistance[j];
				spriteDistance[j] = spriteDistance[i];
				spriteDistance[i] = spriteDistanceTemp;
				// Swap sprites order
				w3d_Sprite* spriteTemp = spriteList->sprite[j];
				spriteList->sprite[j] = spriteList->sprite[i];
				spriteList->sprite[i] = spriteTemp;
				swapped = true;
			}
		}
	}
}
