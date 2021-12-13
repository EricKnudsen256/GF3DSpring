#include "simple_logger.h"

#include "o_finaldoor.h"
#include "p_player.h"

//position is spot on floor for bed, not center of model
Object* final_door_new(Door* door)
{

	Object* finalDoor = object_new();

	finalDoor->position = door->center;
	finalDoor->rotation = door->rotation;

	door->model = gf3d_model_load("door_final");

	finalDoor->hitbox = hitbox_new(door->center, vector3d(0, 0, 0), vector3d(0, 0, 0));
	finalDoor->hitbox->type = HITBOX_WORLD;
	finalDoor->hitbox->parent = finalDoor;

	finalDoor->interactBox = hitbox_new(door->center, vector3d(door->hitbox->dimensions.x + 8, door->hitbox->dimensions.y + 8, door->hitbox->dimensions.z), vector3d(0, 0, 0));
	finalDoor->interactBox->type = HITBOX_INTERACT;
	finalDoor->interactBox->parent = finalDoor;

	finalDoor->interact = final_door_interact;
	finalDoor->free = final_door_free;

	finalDoor->parent = door;

	finalDoor->type = OBJ_EXIT;

	slog("EXIT PLACED: X%f, Y%f", door->center.x, door->center.y);

	return finalDoor;
}

void final_door_interact(Object* obj, Entity* player)
{
	Player* p = player->parent;
	FinalDoor* door = obj->parent;

	slog("WIN");

	
	if (p->win == false)
	{
		p->win = true;
	}

}

void final_door_free(Object* obj)
{
	free(obj->parent);
}