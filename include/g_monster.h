#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "g_entity.h"

#include "p_player.h"

typedef struct
{
	Entity* ent;

	Vector3D movedir;
	Uint32 moveTime;

}Monster;

Monster* monster_new();

void monster_think(Entity* self);

void monster_update(Entity* self);

void monster_draw(Entity* self);

void monster_free(Entity* self);

void monster_move(Vector3D move, Entity* self);

void monster_rotate(float degrees, int axis, Entity* self);


#endif