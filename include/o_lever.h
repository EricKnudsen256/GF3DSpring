#ifndef __LEVER_H__
#define __LEVER_H__

#include "gf3d_model.h"
#include "gfc_types.h"
#include "gfc_vector.h"

#include "g_hitbox.h"

#include "o_object.h"

#include "w_room.h"

typedef struct
{
	Object* obj;
	Bool toggled;

	Model* lever_up;
	Model* lever_down;

	Room* room;

}Lever;

Object* lever_new(Vector3D position, Vector3D rotation, Room *room);

void lever_interact(Object* obj, Entity* player);

void lever_free(Object* obj);





#endif