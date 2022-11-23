
#include "TextureList.h"

struct w3d_TextureListSt {
	w3d_Texture** texture;
	uint16_t size;
};

/*################################################################################*/
w3d_TextureList* w3d_TextureList_Create(uint16_t initialSize) {

	w3d_TextureList* textureList = (w3d_TextureList*)malloc(sizeof(w3d_TextureList));
	if (textureList == NULL) {
		return NULL;
	}
	textureList->size = initialSize;
	textureList->texture = (w3d_Texture**)malloc(initialSize*sizeof(w3d_Texture*));
	if (textureList->texture == NULL) {
		free(textureList);
		return NULL;
	}
    memset(textureList->texture, 0, initialSize*sizeof(w3d_Texture*));

	return textureList;
}

/*################################################################################*/
void w3d_TextureList_Destroy(w3d_TextureList* textureList) {
	assert(textureList != NULL);
	free(textureList->texture);
	free(textureList);
}

/*################################################################################*/
uint16_t w3d_TextureList_Pop(w3d_TextureList* textureList, w3d_Texture* texture) {
	assert(textureList != NULL);
    uint16_t index;
    for (index = 0; index < textureList->size; index++) {
        if (textureList->texture[index] == NULL) {
            textureList->texture[index] = texture;
            break;
        }
    }
    return index;
}

/*################################################################################*/
w3d_Texture* w3d_TextureList_GetByIndex(w3d_TextureList* textureList, uint16_t index) {
	assert(textureList != NULL);
    if (index >= textureList->size) {
        return NULL;
    }
    return textureList->texture[index];
}


