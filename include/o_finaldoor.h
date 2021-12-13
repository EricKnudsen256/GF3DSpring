#ifndef __FINALDOOR_H__
#define __FINALDOOR_H__

#include "gf3d_model.h"
#include "gfc_types.h"
#include "gfc_vector.h"

#include "g_hitbox.h"

#include "o_object.h"

#include "w_room.h"

typedef struct
{
	Object* obj;

}FinalDoor;

Object* final_door_new(Door* door);

void final_door_interact(Object* obj, Entity* player);

void final_door_free(Object* obj);


#endif