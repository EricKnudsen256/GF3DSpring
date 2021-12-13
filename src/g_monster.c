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

    Entity* player = entity_manager_get_player();

    while (!spawned)
    {
        int rnd = random_int_range(0, world.max_rooms);

        if (world.room_list[rnd]._inuse)
        {

            spawnPos = vector3d(world.room_list[rnd].position.x, world.room_list[rnd].position.y, -22);
            spawned = true;
        }
    }

    monster->ent->position = spawnPos;

    entity_make_hitbox(vector3d(8, 8, 13), vector3d(0, 0, 0), monster->ent);

    monster->ent->model = gf3d_model_load_animated("dino_skin", 0, 30);

    gfc_matrix_identity(monster->ent->modelMat); //neded to draw the hitbox
    monster->ent->parent = monster;

    monster->ent->_inuse = 1;
    monster->ent->velocity = vector3d(0, 0, 0);

    monster->ent->_lockmovement = 0;

    monster->moveTime = get_current_time() + 3000;
    monster->movedir = vector3d(0, 0, 0);

    monster->frameRate = 60;

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
         !p->cloaked && !p->hiding)
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

    float deltaTime = (float)get_delta_time() / 1000.0000;
    int currentTime = get_current_time();

    float deltaY, deltaX, angle;

    Entity* player = entity_manager_get_player();
    Monster* m = self->parent;

    if (!self->_lockmovement)
    {
        deltaY = self->position.y - (self->position.y + self->velocity.y);
        deltaX = self->position.x - (self->position.x + self->velocity.x);
        angle = atan2(deltaY, deltaX) - M_PI / 2;
        vector3d_add(self->position, self->position, self->velocity);
    }
    else
    {
        deltaY = self->position.y - (self->position.y + self->velocity.y);
        deltaX = self->position.x - (self->position.x + self->velocity.x);
        angle = atan2(deltaY, deltaX) - M_PI / 2;
    }

    //slog("MONSTER: X:%f, Y:%f, Z:%f", self->position.x, self->position.y, self->position.z);
    

    gfc_matrix_identity(self->modelMat);


        gfc_matrix_make_translation(
            self->modelMat,
            self->position
        );
        //slog("VEL TAN: %f, X: %f, Y: %f, DELTATIME: %f ", angle, self->velocity.x, self->velocity.y, deltaTime);

        gfc_matrix_rotate(self->modelMat, self->modelMat, angle, vector3d(0, 0, 1));

        if (m->attacking)
        {
            if (currentTime > m->lastFrame + (int)(1000.0 / m->frameRate))
            {
                m->currentFrame++;
                m->lastFrame = currentTime;
            }

            if (m->currentFrame >= 30)
            {
                m->currentFrame = 0;
                m->lastFrame = currentTime;
                m->attacking = false;

            }
        }



    if (self->hitbox)
    {
        hitbox_set_pos(self->position, self->hitbox);
    }
}

void monster_draw(Entity* self)
{
    Monster* monster = self->parent;

    //slog("DRAW: x:%f, y:%f, z:%f", self->position.x, self->position.y, self->position.z);

    entity_draw(self, monster->currentFrame);
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