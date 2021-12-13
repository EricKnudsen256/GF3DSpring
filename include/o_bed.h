#ifndef __BED_H__
#define __BED_H__

#include "gf3d_model.h"
#include "gfc_types.h"
#include "gfc_vector.h"

#include "g_hitbox.h"

#include "o_object.h"

typedef struct
{
	Object *obj;
	Bool playerHiding;

}Bed;

Object* bed_new(Vector3D position, Vector3D rotation);

void bed_interact(Object* obj, Entity* player);

void bed_free(Object *obj);





#endif