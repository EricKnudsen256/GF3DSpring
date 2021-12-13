#include "simple_logger.h"

#include "o_locker.h"
#include "p_player.h"

//position is spot on floor for bed, not center of model
Object* locker_new(Vector3D position, Vector3D rotation)
{
	Locker* l;

	l = (Locker*)malloc(sizeof(Locker));

	memset(l, 0, sizeof(Locker));

	Object* locker = object_new();

	locker->position = position;
	locker->rotation = rotation;

	locker->model = gf3d_model_load("locker");
	gfc_matrix_identity(locker->modelMat);
	gfc_matrix_make_translation(locker->modelMat, position);
	gfc_matrix_rotate(locker->modelMat, locker->modelMat, rotation.x, vector3d(1, 0, 0));
	gfc_matrix_rotate(locker->modelMat, locker->modelMat, rotation.z, vector3d(0, 0, 1));

	locker->hitbox = hitbox_new(position, vector3d(12, 12, 20), vector3d(0, 0, 0));
	locker->hitbox->type = HITBOX_WORLD;
	locker->hitbox->parent = locker;

	locker->interactBox = hitbox_new(position, vector3d(22, 22, 30), vector3d(0, 0, 0));
	locker->interactBox->type = HITBOX_INTERACT;
	locker->interactBox->parent = locker;

	locker->interact = locker_interact;
	locker->free = locker_free;

	l->obj = locker;

	locker->parent = l;

	locker->type = OBJ_LOCKER;

	return locker;
}

void locker_interact(Object* obj, Entity* player)
{
	Player* p = player->parent;
	Locker* locker = obj->parent;

	if (locker->playerHiding)
	{
		p->hiding = false;
		locker->playerHiding = false;
		p->hidePos = vector3d(0, 0, 0);
	}
	else
	{
		p->hiding = true;
		locker->playerHiding = true;
		p->hidePos = vector3d(obj->position.x, obj->position.y, obj->position.z + 4);

	}


}

void locker_free(Object* obj)
{
	free(obj->parent);
}