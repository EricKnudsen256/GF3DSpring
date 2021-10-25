#ifndef __HITBOX_H__
#define __HITBOX_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_model.h"

typedef struct
{
	Vector3D center;
	Vector3D dimensions; 

	Model *wireframe;

}Hitbox;


/**
* @brief creates a new hitbox
* @params center the center point of the hitbox related to entity, 0, 0, 0 is in the center of the ent
* @params dimensions the width, height, and depth of the hitbox
* @return the newly created hitbox, NULL on error
*/
Hitbox* hitbox_new(Vector3D center, Vector3D dimensions);

/**
* @brief sets the position of the hitbox
* @params position the new center position of the hitbox
*/
void hitbox_set_pos(Vector3D position, Hitbox* hitbox);

/**
* @brief frees memeory of created hitboxes
* @params self the hitbox to free
*/
void hitbox_free(Hitbox *hitbox);

/**
* @brief frees memeory of created hitboxes
* @params box1 the first box to check
* @params box2 the second box to check
* @return true if collision is detected, false otherwise
*/
Bool hitbox_check_collision(Hitbox *box1, Hitbox *box2);

/**
* @brief draws a hitbox wireframe
* @params self the hitbox to draw
*/
void hitbox_draw(Hitbox* hitbox, Uint32 bufferFrame, VkCommandBuffer commandBuffer, Matrix4 modelMat);

#endif