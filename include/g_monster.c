#include "simple_logger.h"

#include "g_monster.h"
#include "g_random.h"
#include "g_time.h"

#include "w_world.h"

Monster* monster_new()
{
    Bool spawned = false;

    Vector3D spawnPos;

    Monster* monster = (Monster*)malloc(sizeof(Monster));
    memset(monster, 0, sizeof(Monster));

    monster->ent = entity_new();
    monster->ent->update = monster_update;
    monster->ent->think = monster_think;
    monster->ent->draw = monster_draw;

    //set position

    World world = world_get();

    while (!spawned)
    {
        int rnd = random_int_range(0, world.max_rooms);

        if (world.room_list[rnd]._inuse)
        {
            spawnPos = vector3d(world.room_list[rnd].position.x, world.room_list[rnd].position.y, world.room_list[rnd].position.z);
            spawned = true;
        }
    }

    monster->ent->position = spawnPos;

    //entity_make_hitbox(vector3d(8, 8, 20), vector3d(0, 0, 0), monster->ent);

    monster->ent->model = gf3d_model_load("dino");

    gfc_matrix_identity(monster->ent->modelMat); //neded to draw the hitbox
    monster->ent->parent = monster;

    monster->ent->_inuse = 1;

    return monster;
}

void monster_think(Entity* self)
{
    Monster* monster = self->parent;

    if (monster->moveTime > get_current_time())
    {
        monster->moveTime = get_current_time() + random_int_range(5000, 10000);
        monster->movedir = vector3d(random_float(-1, 1), random_float(-1, 1), 0);

        vector3d_normalize(&monster->movedir);

        monster->ent->velocity = monster->movedir;
    }
}

void monster_update(Entity* self)
{
    vector3d_add(self->position, self->position, self->velocity);

    gfc_matrix_identity(self->modelMat);

    gfc_matrix_make_translation(
        self->modelMat,
        self->position
    );

    if (self->hitbox)
    {
        hitbox_set_pos(self->position, self->hitbox);
    }
}

void monster_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer, Entity* self)
{
    slog("DRAW: x:%f, y:%f, z:%f", self->position.x, self->position.y, self->position.z);

    entity_draw(bufferFrame, commandBuffer, self);
}

void monster_free(Entity* self)
{

}

void monster_move(Vector3D move, Entity* self)
{

}

void monster_rotate(float degrees, int axis, Entity* self)
{

}