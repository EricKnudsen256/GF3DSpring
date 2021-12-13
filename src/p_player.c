#include "simple_logger.h"

#include "gfc_matrix.h"

#include "g_time.h"


#include "p_player.h"
#include "g_monster.h"


Player* player_new(Vector3D spawnPos)
{
    Player* player = (Player*)malloc(sizeof(Player));
    memset(player, 0, sizeof(Player));

    player->ent = entity_new();
    player->ent->update = player_update;
    player->ent->think = player_think;
    player->ent->draw = player_draw;
    player->ent->free = player_free;
    player->ent->kill = player_kill;
    player->ent->position = spawnPos;
    entity_make_hitbox(vector3d(8, 8, 20), vector3d(0, 0, 0), player->ent);

    gfc_matrix_identity(player->ent->modelMat); //neded to draw the hitbox
    player->ent->parent = player;

    player->camOffset = 5;
    player->ent->dead = false;
    player->ent->health = 100;

    player->ent->_inuse = 1;
    player->ent->type = ENT_PLAYER;

    player->cloaked = false;
    player->cloaksLeft = 2;

    player->interactCooldown = 1000;

    player->light = light_new(player->ent->position);

    light_set_color(vector4d(1.0, .9, .6, .01), player->light);
    light_set_ambient_color(vector4d(0, 0, 0, 0), player->light);

    return player;
}

void player_think(Entity* self)
{
    //Bool moved = false;
    int x, y;

    Player* player = self->parent;

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    float deltaTime = (float)get_delta_time() / 1000.0;

    //slog("DeltaTime: %f", deltaTime);

    SDL_GetRelativeMouseState(&x, &y);

    gf3d_camera_angle_vectors(self->rotation, &self->forward, &self->right, &self->up);

    self->back = vector3d(-self->forward.x, -self->forward.y, -self->forward.z);
    self->left = vector3d(-self->right.x, -self->right.y, -self->right.z);
    self->down = vector3d(-self->up.x, -self->up.y, -self->up.z);

    player->camOffset = 5;
    player->crouched = false;

    if (keys[SDL_SCANCODE_LSHIFT])
    {
        vector3d_scale(self->forward, self->forward, deltaTime * 60);
        vector3d_scale(self->back, self->back, deltaTime * 60);
        vector3d_scale(self->right, self->right, deltaTime * 60);
        vector3d_scale(self->left, self->left, deltaTime * 60);
        vector3d_scale(self->up, self->up, deltaTime * 60);
        vector3d_scale(self->down, self->down, deltaTime * 60);
    }
    else if (keys[SDL_SCANCODE_LCTRL])
    {
        player->camOffset = -3;
        player->crouched = true;

        vector3d_scale(self->forward, self->forward, deltaTime * 20);
        vector3d_scale(self->back, self->back, deltaTime * 20);
        vector3d_scale(self->right, self->right, deltaTime * 20);
        vector3d_scale(self->left, self->left, deltaTime * 20);
        vector3d_scale(self->up, self->up, deltaTime * 20);
        vector3d_scale(self->down, self->down, deltaTime * 20);
    }
    else
    {
        vector3d_scale(self->forward, self->forward, deltaTime * 30);
        vector3d_scale(self->back, self->back, deltaTime * 30);
        vector3d_scale(self->right, self->right, deltaTime * 30);
        vector3d_scale(self->left, self->left, deltaTime * 30);
        vector3d_scale(self->up, self->up, deltaTime * 30);
        vector3d_scale(self->down, self->down, deltaTime * 30);
    }


    if (keys[SDL_SCANCODE_C] && player->cloaksLeft > 0)
    {
        player->cloaksLeft--;

        player->cloakTime = get_current_time() + 10000;
        player->cloaked = true;
    }

    if (player->cloakTime < get_current_time())
    {
        player->cloaked = false;
    }


    if (x != 0)
    {
        player_rotate(.7 * deltaTime * x, 2, self);
    }
    if (y != 0)
    {
        player_rotate(.7 * deltaTime * y, 0, self);
    }

    self->velocity = vector3d(0, 0, 0);


    if (keys[SDL_SCANCODE_W])
    {
        player_move(self->forward, self);
        //moved = true;
    }
    else if (keys[SDL_SCANCODE_S])
    {
        player_move(self->back, self);
        //moved = true;
    }

    if (keys[SDL_SCANCODE_A])
    {
        player_move(self->left, self);
        //moved = true;
    }
    else if (keys[SDL_SCANCODE_D])
    {
        player_move(self->right, self);
        //moved = true;
    }

    if (keys[SDL_SCANCODE_SPACE])
    {
        player_move(self->up, self);
        //moved = true;
    }

    else if (keys[SDL_SCANCODE_Z])
    {
        player_move(self->down, self);
        //moved = true;
    }

    if (keys[SDL_SCANCODE_E] && get_current_time() >= player->interactTime + player->interactCooldown)
    {
        player->checkInteract = true;
        player->interactTime = get_current_time();
        //moved = true;
    }
    else
    {
        player->checkInteract = false;
    }

    


    if (self->rotation.x > .9)
    {
        self->rotation.x = .9;
    }
    else if (self->rotation.x < -.9)
    {
        self->rotation.x = -.9;
    }


}

void player_update(Entity* self)
{
    //Camera camera = gf3d_get_camera();

    Player* player = self->parent;



    if (!player->ent->dead || player->win)
    {
        if (player->hiding)
        {
            gf3d_camera_set_position(player->hidePos);
            gf3d_camera_set_rotation(self->rotation);
        }
        else
        {
            gf3d_camera_set_position(vector3d(self->position.x, self->position.y, self->position.z + player->camOffset));
            gf3d_camera_set_rotation(self->rotation);

            vector3d_add(self->position, self->position, self->velocity);
        }

        entity_update(self);

    }

    light_set_pos(vector3d(self->position.x, self->position.y, self->position.z + 4), player->light);

    if (!player->win)
    {
        //slog("PLAYER X:%f, Y:%f", self->position.x, self->position.y);
    }


}

void player_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer, Entity* self)
{

}

void player_free(Entity* self)
{
    if (!self)
    {
        return;
    }
    if (!self->parent)
    {
        return;
    }

    Player* player = self->parent;
    entity_free(self);

    free(player);
}

void player_move(Vector3D move, Entity* self)
{
    self->velocity.x += move.x;
    self->velocity.y += move.y;
    self->velocity.z += move.z;
}

void player_rotate(float degrees, int axis, Entity* self)
{
    if (axis == 0)
    {
        self->rotation.x += degrees;
    }
    else if (axis == 2)
    {
        self->rotation.z += degrees;
    }
}
void player_kill(Entity* self)
{
    Player* player = self->parent;
    Entity* m = entity_manager_get_monster();

    Monster* monster = m->parent;

    if (player->ent->dead)
    {
        return;
    }

    if (!player->cloaked && !player->hiding && !player->win)
    {
        player->ent->dead = true;

        player->ent->rotation.x = 0;
        gf3d_camera_set_rotation(self->rotation);
        monster->ent->_lockmovement = 1;

        //play death anim

        Vector3D normalForward = player->ent->forward;

        vector3d_normalize(&normalForward);

        slog("Z:%f", player->ent->rotation.z);

        monster->ent->position = vector3d(player->ent->position.x + normalForward.x * 7, player->ent->position.y + normalForward.y * 7, monster->ent->position.z + 5);
        monster->attacking = true;
        monster->lastFrame = get_current_time();
     

    }
}

