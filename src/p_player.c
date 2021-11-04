#include "simple_logger.h"

#include "gfc_matrix.h"

#include "p_player.h"


Player* player_new(Vector3D spawnPos)
{
    Player* player = (Player*)malloc(sizeof(Player));
    memset(player, 0, sizeof(Player));

    player->ent = entity_new();
    player->ent->update = player_update;
    player->ent->think = player_think;
    player->ent->draw = player_draw;
    entity_make_hitbox(vector3d(10, 10, 20), vector3d(0, 0, 0), player->ent);

    gfc_matrix_identity(player->ent->modelMat); //neded to draw the hitbox
    player->ent->parent = player;

    return player;
}

void player_think(Entity* self)
{
    Bool moved = false;
    int x, y;

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    SDL_GetRelativeMouseState(&x, &y);

    gf3d_camera_angle_vectors(self->rotation, &self->forward, &self->right, &self->up);

    self->back = vector3d(-self->forward.x, -self->forward.y, -self->forward.z);
    self->left = vector3d(-self->right.x, -self->right.y, -self->right.z);
    self->down = vector3d(-self->up.x, -self->up.y, -self->up.z);


    vector3d_scale(self->forward, self->forward, .01);
    vector3d_scale(self->back, self->back, .01);
    vector3d_scale(self->right, self->right, .01);
    vector3d_scale(self->left, self->left, .01);
    vector3d_scale(self->up, self->up, .01);
    vector3d_scale(self->down, self->down, .01);

    if (x != 0)
    {
        player_rotate(0.001 * x, 2, self);
    }
    if (y != 0)
    {
        player_rotate(0.001 * y, 0, self);
    }

    self->velocity = vector3d(0, 0, 0);


    if (keys[SDL_SCANCODE_W])
    {
        player_move(self->forward, self);
        moved = true;
    }
    else if (keys[SDL_SCANCODE_S])
    {
        player_move(self->back, self);
        moved = true;
    }

    if (keys[SDL_SCANCODE_A])
    {
        player_move(self->left, self);
        moved = true;
    }
    else if (keys[SDL_SCANCODE_D])
    {
        player_move(self->right, self);
        moved = true;
    }

    if (keys[SDL_SCANCODE_SPACE])
    {
        player_move(self->up, self);
        moved = true;
    }
    else if (keys[SDL_SCANCODE_LSHIFT])
    {
        player_move(self->down, self);
        moved = true;
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
    Camera camera = gf3d_get_camera();

    gf3d_camera_set_position(vector3d(self->position.x, self->position.y, self->position.z + 5));
    gf3d_camera_set_rotation(self->rotation);

    vector3d_add(self->position, self->position, self->velocity);

    //slog("Rotation: x:%f, y:%f, z:%f", self->rotation.x, self->rotation.y, self->rotation.z);

    entity_update(self);
}

void player_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer, Entity* self)
{

}

void player_free(Entity* self)
{

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
