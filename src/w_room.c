#include "simple_logger.h"

#include "w_room.h"

void room_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer, Room *room)
{
    if (!room)
    {
        slog("No room given");
        return;
    }

    if (!room->model) return;
    if (!room->modelMat) return;

    gf3d_model_draw(room->model, bufferFrame, commandBuffer, room->modelMat);
}

void room_make_hitboxs(Room* room)
{
    if (!room) return;
    if (room->hitbox_list)
    {
        room->hitbox_list = NULL;
    }

    room->hitbox_list = gfc_allocate_array(sizeof(Hitbox), 10);
    room->hitbox_max = 10;

    //test room hitbox to see if scale is right
    room->hitbox_list[0] = hitbox_new(vector3d(0, 0, 0), vector3d(33.725, 33.725, 1), vector3d(0, 0, -33.725/2));
    room->hitbox_list[0]->parent = room;

    room->hitbox_list[1] = hitbox_new(vector3d(0, 0, 0), vector3d(8, 8, 33.725), vector3d(33.725 / 2 - 4, 33.725 / 2 - 4, 0));
    room->hitbox_list[1]->parent = room;
    //slog("made room hitboxes");
}
void room_set_position(Vector3D pos, Room* room)
{
    if (!room)
    {
        slog("No room given");
        return;
    }
    if (room->modelMat)
    {
        room->modelMat[3][0] = pos.x;
        room->modelMat[3][1] = pos.y;
        room->modelMat[3][2] = pos.z;
    }
    room->position.x = pos.x;
    room->position.y = pos.y;
    room->position.z = pos.z;

    for (int i = 0; i < room->hitbox_max; i++)
    {
        if (room->hitbox_list[i])
        {
            //slog("Room pos moved");
            hitbox_set_pos(pos, room->hitbox_list[i]);
        }
    }
    
}
