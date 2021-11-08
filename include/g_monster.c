#include "simple_logger.h"

#include "g_monster.h"
#include "g_random.h"
#include "g_time.h"

#include "p_player.h"

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
            spawnPos = vector3d(world.room_list[rnd].position.x, world.room_list[rnd].position.y, -8);
            spawned = true;
        }
    }

    monster->ent->position = spawnPos;

    entity_make_hitbox(vector3d(8, 8, 13), vector3d(0, 0, 0), monster->ent);

    monster->ent->model = gf3d_model_load("dino");

    gfc_matrix_identity(monster->ent->modelMat); //neded to draw the hitbox
    monster->ent->parent = monster;

    monster->ent->_inuse = 1;
    monster->ent->velocity = vector3d(0, 0, 0);

    monster->moveTime = get_current_time() + 3000;
    monster->movedir = vector3d(0, 0, 0);

    monster->ent->type = ENT_MONSTER;


    return monster;
}

void monster_think(Entity* self)
{
    Monster* monster = self->parent;
    float deltaTime = (float)get_delta_time() / 1000.0000;

    Entity* player = entity_manager_get_player();
    Player* p = player->parent;

    Vector3D playerDir;

    if(((vector3d_distance_between_less_than(player->position, monster->ent->position, 100) && !p->crouched) || 
        (vector3d_distance_between_less_than(player->position, monster->ent->position, 50) && p->crouched)) &&
       !player->dead && !p->cloaked)
    {
        vector3d_sub(playerDir, player->position, monster->ent->position);
        playerDir = vector3d(playerDir.x, playerDir.y, 0);

        vector3d_normalize(&playerDir);

        if (deltaTime == 0.000)
        {
            deltaTime = 0.001;
        }

        vector3d_scale(playerDir, playerDir, deltaTime * 20);

        monster->ent->velocity = playerDir;
        monster->movedir = playerDir;

        //slog("playerDir: x:%f, y:%f", playerDir.x, playerDir.y);
    }
    
    else if (monster->moveTime < get_current_time())
    {
        //slog("Change dir");

        monster->moveTime = get_current_time() + random_int_range(5000, 10000);
        monster->movedir = vector3d(random_float(-1, 1), random_float(-1, 1), 0);

        monster->ent->velocity = monster->movedir;

        vector3d_normalize(&monster->ent->velocity);

        vector3d_scale(monster->ent->velocity, monster->ent->velocity, deltaTime * 20);
        //slog("MOVEDIR: X:%f, Y:%f", monster->movedir.x, monster->movedir.y);

        
    }
    
    
}

void monster_update(Entity* self)
{
    //Monster* monster = self->parent;
    //Entity* player = entity_manager_get_player();

    float deltaTime = (float)get_delta_time() / 1000.0000;

    float deltaY, deltaX, angle;

    deltaY = self->position.y - (self->position.y + self->velocity.y);
    deltaX = self->position.x - (self->position.x + self->velocity.x);
    angle = atan2(deltaY, deltaX) - M_PI / 2;

    vector3d_add(self->position, self->position, self->velocity);

    gfc_matrix_identity(self->modelMat);

    gfc_matrix_make_translation(
        self->modelMat,
        self->position
    );
   //slog("VEL TAN: %f, X: %f, Y: %f, DELTATIME: %f ", angle, self->velocity.x, self->velocity.y, deltaTime);

    gfc_matrix_rotate(self->modelMat, self->modelMat, angle, vector3d(0, 0, 1));


    if (self->hitbox)
    {
        hitbox_set_pos(self->position, self->hitbox);
    }
}

void monster_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer, Entity* self)
{
    //slog("DRAW: x:%f, y:%f, z:%f", self->position.x, self->position.y, self->position.z);

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