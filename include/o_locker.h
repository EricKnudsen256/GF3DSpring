#ifndef __LOCKER_H__
#define __LOCKER_H__

#include "gf3d_model.h"
#include "gfc_types.h"
#include "gfc_vector.h"

#include "g_hitbox.h"

#include "o_object.h"

typedef struct
{
	Object* obj;
	Bool playerHiding;

}Locker;

Object* locker_new(Vector3D position, Vector3D rotation);

void locker_interact(Object* obj, Entity* player);

void locker_free(Object* obj);

#endif

