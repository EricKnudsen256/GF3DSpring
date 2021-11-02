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
    hitbox->center = center;

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

	hitbox->center = vector3d(position.x + hitbox->offset.x, position.y + hitbox->offset.y, position.z + hitbox->offset.z);
	//slog("Center: x:%f, y:%f, z:%f", hitbox->center.x, hitbox->center.y, hitbox->center.z);
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


Bool hitbox_check_collision(Hitbox* box1, Hitbox* box2, Vector3D box1velocity)
{
	//todo later after hitbox drawing works properly

	if (box1->center.x + (box1->dimensions.x / 2) + box1velocity.x <= box2->center.x - (box2->dimensions.x / 2))
	{
		return false;
	}
	if (box2->center.x + (box2->dimensions.x / 2) <= box1->center.x - (box1->dimensions.x / 2) + box1velocity.x)
	{
		return false;
	}
	if (box1->center.y + (box1->dimensions.y / 2) + box1velocity.y <= box2->center.y - (box2->dimensions.y / 2))
	{
		return false;
	}
	if (box2->center.y + (box2->dimensions.y / 2) <= box1->center.y - (box1->dimensions.y / 2) + box1velocity.y)
	{
		return false;
	}
	if (box1->center.z + (box1->dimensions.z / 2) + box1velocity.z <= box2->center.z - (box2->dimensions.z / 2))
	{
		return false;
	}
	if (box2->center.z + (box2->dimensions.z / 2) <= box1->center.z + box1velocity.z - (box1->dimensions.z / 2))
	{
		return false;
	}

	return true;


}


void hitbox_draw(Hitbox* hitbox, Uint32 bufferFrame, VkCommandBuffer commandBuffer, Matrix4 modelMat)
{

    Matrix4 hitboxMat;

	//slog("offset: %f, %f, %f", hitbox->offset.x, hitbox->offset.y, hitbox->offset.z);
    
    //gfc_matrix_copy(hitboxMat, modelMat);
    
    gfc_matrix_identity(hitboxMat);

	hitboxMat[3][0] = modelMat[3][0];
	hitboxMat[3][1] = modelMat[3][1];
	hitboxMat[3][2] = modelMat[3][2];
	
	
	hitboxMat[3][0] += hitbox->offset.x;
	hitboxMat[3][1] += hitbox->offset.y;
	hitboxMat[3][2] += hitbox->offset.z;
	

		
	gf3d_model_draw(hitbox->wireframe, bufferFrame, commandBuffer, hitboxMat);

}
