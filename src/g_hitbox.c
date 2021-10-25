#include "simple_logger.h"

#include "g_hitbox.h"
#include "gf3d_pipeline.h"




Hitbox* hitbox_new(Vector3D center, Vector3D dimensions)
{
	Hitbox* hitbox;

	hitbox = (Hitbox*)malloc(sizeof(Hitbox));

	memset(hitbox, 0, sizeof(Hitbox));


	if (!hitbox)
	{
		slog("Hitbox not created properly");
		return NULL;
	}

	hitbox->center = center;
	hitbox->dimensions = dimensions;

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

	hitbox->center = position;
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

	/*
	VkDescriptorSet* descriptorSet = NULL;
	Pipeline *pipe;

	pipe = gf3d_vgraphics_get_wireframe_pipeline();

	if (!hitbox)
	{
		slog("cannot render a NULL hitbox");
		return;
	}
	descriptorSet = gf3d_pipeline_get_descriptor_set(pipe, bufferFrame);
	if (descriptorSet == NULL)
	{
		slog("failed to get a free descriptor Set for model rendering");
		return;
	}
	gf3d_model_update_basic_model_descriptor_set(hitbox->wireframe, *descriptorSet, bufferFrame, modelMat);
	gf3d_mesh_render(hitbox->wireframe, commandBuffer, descriptorSet);
	*/

	gf3d_model_draw(hitbox->wireframe, bufferFrame, commandBuffer, modelMat);

}
