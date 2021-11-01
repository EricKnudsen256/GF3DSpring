#include "simple_logger.h"

#include "g_hitbox.h"
#include "gf3d_pipeline.h"




Hitbox* hitbox_new(Vector3D center, Vector3D dimensions, Vector3D offset)
{
	Hitbox* hitbox;

	hitbox = (Hitbox*)malloc(sizeof(Hitbox));

	memset(hitbox, 0, sizeof(Hitbox));


	if (!hitbox)
	{
		slog("Hitbox not created properly");
		return NULL;
	}

	hitbox->dimensions = dimensions;
    hitbox->offset = offset;
    hitbox->center = vector3d(center.x - offset.x, center.y - offset.y, center.z - offset.z);

	hitbox->wireframe = gf3d_model_from_hitbox(hitbox->dimensions);

	return hitbox;
}


void hitbox_set_pos(Vector3D position, Hitbox* hitbox)
{
	if (!hitbox)
	{
		slog("Cannot set position of NULL hitbox");
		return;
	}

	hitbox->center = vector3d(position.x - hitbox->offset.x, position.y - hitbox->offset.y, position.z - hitbox->offset.z);
}


void hitbox_free(Hitbox* hitbox)
{
	if (!hitbox)
	{
		slog("Cannot free NULL hitbox");
		return;
	}

	free(hitbox);
}


Bool hitbox_check_collision(Hitbox* box1, Hitbox* box2)
{
	//todo later after hitbox drawing works properly
}


void hitbox_draw(Hitbox* hitbox, Uint32 bufferFrame, VkCommandBuffer commandBuffer, Matrix4 modelMat)
{

    Matrix4 hitboxMat;
    
    //gfc_matrix_copy(hitboxMat, modelMat);
    
    gfc_matrix_identity(hitboxMat);
    
    gfc_matrix_make_translation(
        hitboxMat,
            hitbox->center
        );
    
        gfc_matrix_make_translation(
        hitboxMat,
            hitbox->offset
        );
        
	gf3d_model_draw(hitbox->wireframe, bufferFrame, commandBuffer, hitboxMat);

}
