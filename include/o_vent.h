#ifndef __VENT_H__
#define __VENT_H__

#include "gf3d_model.h"
#include "gfc_types.h"
#include "gfc_vector.h"

#include "g_hitbox.h"

#include "o_object.h"

#include "w_room.h"

typedef struct 
{
	Object* obj;
	Vector3D exitPos;
	Object* connectedVent;

}Vent;

Object* vent_new(Vector3D position, Vector3D rotation, Room* room);

void vent_interact(Object* obj, Entity* player);

void vent_connect_all();

void vent_free(Object* obj);


#endif