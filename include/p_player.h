#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gf3d_camera.h"

#include "g_entity.h"


typedef struct
{
	Entity* ent;
	//Camera* camera;

}Player;

Player* player_new(Vector3D spawnPos);

void player_think(Entity* self);

void player_update(Entity* self);

void player_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer, Entity* self);

void player_free(Entity* self);

void player_move(Vector3D move, Entity* self);

void player_rotate(float degrees, int axis, Entity* self);


#endif