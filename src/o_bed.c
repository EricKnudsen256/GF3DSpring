#include "simple_logger.h"

#include "o_bed.h"
#include "p_player.h"

//position is spot on floor for bed, not center of model
Object* bed_new(Vector3D position, Vector3D rotation) 
{
	Bed* b;

	b = (Bed*)malloc(sizeof(Bed));

	memset(b, 0, sizeof(Bed));

	Object* bed = object_new();

	bed->position = position;
	bed->rotation = rotation;

	bed->model = gf3d_model_load("bed");
	gfc_matrix_identity(bed->modelMat);
	gfc_matrix_make_translation(bed->modelMat, position);
	gfc_matrix_rotate(bed->modelMat, bed->modelMat, rotation.x, vector3d(1, 0, 0));
	gfc_matrix_rotate(bed->modelMat, bed->modelMat, rotation.z, vector3d(0, 0, 1));

	bed->hitbox = hitbox_new(position, vector3d(6, 12, 4), vector3d(0, 0, 0));
	bed->hitbox->type = HITBOX_WORLD;
	bed->hitbox->parent = bed;

	bed->interactBox = hitbox_new(position, vector3d(12, 18, 10), vector3d(0, 0, 0));
	bed->interactBox->type = HITBOX_INTERACT;
	bed->interactBox->parent = bed;

	bed->interact = bed_interact;
	bed->free = bed_free;

	b->obj = bed;

	bed->parent = b;

	bed->type = OBJ_BED;

	return bed;
}

void bed_interact(Object* obj, Entity* player)
{
	Player* p = player->parent;
	Bed* bed = obj->parent;

	if (bed->playerHiding)
	{
		p->hiding = false;
		bed->playerHiding = false;
		p->hidePos = vector3d(0, 0, 0);
	}
	else
	{
		p->hiding = true;
		bed->playerHiding = true;
		p->hidePos = vector3d(obj->position.x, obj->position.y, obj->position.z - 2);
		
	}


}

void bed_free(Object* obj)
{
	free(obj->parent);
}