#ifndef __HITBOX_H__
#define __HITBOX_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_model.h"

typedef enum
{
	HITBOX_UNDEFINED,
	HITBOX_ENTITY,
	HITBOX_WORLD,
	HITBOX_DOOR

}HitboxType;

typedef struct
{

	Vector3D center;		/**center of the hitbox in world space */
	Vector3D dimensions;	/**Size of hitbox in width, depth, height */
    
    Vector3D offset;		/**offset of hitbox from center of object it is attached to */

	Model* wireframe;		/**mesh to draw the wireframe of the hitbox */

	HitboxType type;


	void* parent;

}Hitbox;


/**
* @brief creates a new hitbox
* @params center the center point of the hitbox related to entity, 0, 0, 0 is in the center of the ent
* @params dimensions the width, height, and depth of the hitbox
* @return the newly created hitbox, NULL on error
*/
Hitbox* hitbox_new(Vector3D center, Vector3D dimensions, Vector3D offset);

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
* @brief checks a singular collision to see if two hitboxes are colliding
* @params box1 the first box to check
* @params box2 the second box to check
* @return true if collision is detected, false otherwise
*/
Bool hitbox_check_collision(Hitbox* box1, Hitbox* box2, Vector3D box1velocity);

/**
* @brief draws a hitbox wireframe
* @params self the hitbox to draw
*/
void hitbox_draw(Hitbox* hitbox, Matrix4 modelMat);


void hitbox_draw_door(Hitbox* hitbox, Matrix4 modelMat);
#endif
