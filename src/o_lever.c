#include "simple_logger.h"

#include "o_lever.h"
#include "p_player.h"

//position is spot on floor for bed, not center of model
Object* lever_new(Vector3D position, Vector3D rotation, Room* room)
{
	Lever* l;

	l = (Lever*)malloc(sizeof(Lever));

	memset(l, 0, sizeof(Lever));

	Object* lever = object_new();

	lever->position = position;
	lever->rotation = rotation;

	l->lever_up = gf3d_model_load("lever_up");
	l->lever_down = gf3d_model_load("lever_down");
	lever->model = l->lever_up;

	gfc_matrix_identity(lever->modelMat);
	gfc_matrix_make_translation(lever->modelMat, position);
	gfc_matrix_rotate(lever->modelMat, lever->modelMat, rotation.x, vector3d(1, 0, 0));
	gfc_matrix_rotate(lever->modelMat, lever->modelMat, rotation.z, vector3d(0, 0, 1));

	lever->hitbox = hitbox_new(position, vector3d(0, 0, 0), vector3d(0, 0, 0));
	lever->hitbox->type = HITBOX_WORLD;
	lever->hitbox->parent = lever;

	lever->interactBox = hitbox_new(position, vector3d(10, 10, 10), vector3d(0, 0, 0));
	lever->interactBox->type = HITBOX_INTERACT;
	lever->interactBox->parent = lever;

	lever->interact = lever_interact;
	lever->free = lever_free;

	lever->parent = l;

	l->room = room;

	l->obj = lever;

	lever->type = OBJ_LEVER;

	//setup doors controlled by lever
	if (room->editorRoom) return lever;

	for (int i = 0; i < room->max_doors; i++)
	{
		room->door_list[i].locked = true;
	}

	return lever;
}

void lever_interact(Object* obj, Entity* player)
{
	Player* p = player->parent;
	Lever* lever = obj->parent;

	


	if (!lever->toggled) //lever off
	{
		lever->obj->model = lever->lever_down;
		lever->toggled = true;

		if (lever->room->editorRoom) return;
		for (int i = 0; i < lever->room->max_doors; i++)
		{
			if (lever->room->door_list[i].locked == true)
			{
				//slog("Door close");
				lever->room->door_list[i].hitbox->type = HITBOX_DOOR;
			}
		}
	}
	else if (lever->toggled) //lever on
	{
		lever->obj->model = lever->lever_up;
		lever->toggled = false;
		if (lever->room->editorRoom) return;
		for (int i = 0; i < lever->room->max_doors; i++)
		{
			if (lever->room->door_list[i].locked == true)
			{
				//slog("Door open");
				lever->room->door_list[i].hitbox->type = HITBOX_INACTIVE;
			}
		}
	}


}

void lever_free(Object* obj)
{
	free(obj->parent);
}
