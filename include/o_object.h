#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "gf3d_model.h"
#include "gfc_types.h"
#include "gfc_vector.h"

#include "g_hitbox.h"
#include "g_entity.h"

typedef struct Object_s
{
	Uint32 _id;

	Vector3D position;
	Vector3D rotation;

	Model* model;
	Matrix4 modelMat;       /**4d matrix for displaying the entity */

	Hitbox* hitbox;
	Hitbox* interactBox;

	void(*interact)(struct Object_s* obj, Entity* player);

	void* parent;

}Object;

typedef struct
{
	Object** object_list;
	Uint32 max_objects;

}ObjectManager;

/**
* @brief initializes the object system
* @params max_entities the maximum number of objects to support
*/
void object_manager_init(Uint32 max_objects);


void entity_manager_draw();

/**
* @brief if the object has a hitbox, will call the relevant draw command to show it
*/
void object_manager_draw_hitboxes();

/**
* @brief frees the object manager subsystem
*/
void object_manager_free();

Object* object_new();

void object_draw(Object* obj);

void object_free(Object* obj);


#endif
