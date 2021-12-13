#include "simple_logger.h"

#include "g_random.h"

#include "o_vent.h"
#include "p_player.h"

//position is spot on floor for bed, not center of model
Object* vent_new(Vector3D position, Vector3D rotation, Room* room)
{
	Vent* v;

	v = (Vent*)malloc(sizeof(Vent));

	memset(v, 0, sizeof(Vent));

	Object* vent = object_new();

	vent->position = position;
	vent->rotation = rotation;


	vent->model = gf3d_model_load("vent");

	gfc_matrix_identity(vent->modelMat);
	gfc_matrix_make_translation(vent->modelMat, position);
	gfc_matrix_rotate(vent->modelMat, vent->modelMat, rotation.x, vector3d(1, 0, 0));
	gfc_matrix_rotate(vent->modelMat, vent->modelMat, rotation.z, vector3d(0, 0, 1));

	vent->hitbox = hitbox_new(position, vector3d(0, 0, 0), vector3d(0, 0, 0));
	vent->hitbox->type = HITBOX_WORLD;
	vent->hitbox->parent = vent;

	vent->interactBox = hitbox_new(position, vector3d(10, 10, 8), vector3d(0, 0, 0));
	vent->interactBox->type = HITBOX_INTERACT;
	vent->interactBox->parent = vent;

	vent->interact = vent_interact;
	vent->free = vent_free;

	vent->parent = v;

	v->obj = vent;

	vent->type = OBJ_VENT;

	Vector3D forward = vector3d(0,0,0);

	float sy = sin(rotation.z);
	float cy = cos(rotation.z);

	forward.x = sy;
	forward.y = cy;
	forward.z = 0;

	vector3d_normalize(&forward);

	slog("Vent Forward: X:%f, Y:%f", forward.x, forward.y);

	v->exitPos = vector3d(-forward.x * 8 + position.x,  forward.y * 8 + position.y, -19);


	return vent;
}

void vent_connect_all()
{
	Object* object_list = object_manager_get_object_list();
	Uint32 object_max = object_manager_get_object_max();

	Uint32 ventCount = 0;
	Uint32 lastIndex = -1;

	Uint32 ventsInList = 0;

	

	for (int i = 0; i < object_max; i++)
	{
		if (!object_list[i]._inuse) continue;
		if (object_list[i].type == OBJ_VENT)
		{
			ventCount++;
			lastIndex = i;
		}
	}

	if (lastIndex == -1)
	{
		return;
	}

	int* ventIndicies = gfc_allocate_array(sizeof(int), ventCount);


	
	if (ventCount % 2 == 1)
	{
		object_list[lastIndex]._inuse = 0;
	}

	for (int i = 0; i < object_max; i++)
	{
		if (!object_list[i]._inuse) continue;
		if (object_list[i].type == OBJ_VENT)
		{
			ventIndicies[ventsInList] = i;
			ventsInList++;
		}
	}

	for (int i = 0; i < ventsInList; i++)
	{
		Bool connected = false;
		Vent* ventCurrent = object_list[ventIndicies[i]].parent;


		if (ventCurrent->connectedVent) continue;

		while (!connected)
		{
			int rnd = random_int_range(0, ventsInList - 1);
			Vent* ventOther = object_list[ventIndicies[rnd]].parent;


			if (ventOther->connectedVent) continue;

			ventOther->connectedVent = ventCurrent->obj;
			ventCurrent->connectedVent = ventOther->obj;

			connected = true;
		}
	}




}

void vent_interact(Object* obj, Entity* player)
{
	Player* p = player->parent;
	Vent* vent = obj->parent;
	Vent* connected = vent->connectedVent->parent;

	
	player->position = connected->exitPos;

}

void vent_free(Object* obj)
{
	free(obj->parent);
}
