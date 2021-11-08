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
    room_draw_doors(bufferFrame, commandBuffer, room);
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

    room->hitbox_list[1] = hitbox_new(vector3d(0, 0, 0), vector3d(6, 6, 33.725), vector3d(33.725 / 2 - 3, 33.725 / 2 - 3, 0));
    room->hitbox_list[1]->parent = room;

    room->hitbox_list[2] = hitbox_new(vector3d(0, 0, 0), vector3d(6, 6, 33.725), vector3d(-(33.725 / 2 - 3), 33.725 / 2 - 3, 0));
    room->hitbox_list[2]->parent = room;

    room->hitbox_list[3] = hitbox_new(vector3d(0, 0, 0), vector3d(6, 6, 33.725), vector3d(-(33.725 / 2 - 3), -(33.725 / 2 - 3), 0));
    room->hitbox_list[3]->parent = room;

    room->hitbox_list[4] = hitbox_new(vector3d(0, 0, 0), vector3d(6, 6, 33.725), vector3d(33.725 / 2 - 3, -(33.725 / 2 - 3), 0));
    room->hitbox_list[4]->parent = room;
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

Door* room_new_door(DoorType type, Room* room)
{
    int i;

    if (!room)
    {
        slog("No room given");
        return NULL;
    }
    if (!room->door_list)
    {
        slog("No door list for room");
        return NULL;
    }

    for (i = 0; i < room->max_doors; i++)
    {
        if (!room->door_list[i]._inuse)
        {
            break;
        }
    }
    if (i == room->max_doors)
    {
        slog("No free doors in door");
        return NULL;
    }

    room->door_list[i]._inuse = true;
    room->door_list[i].connected = false;

    switch (type)
    {
        case DOOR_NEG_X:
            room->door_list[i].center = vector3d(room->position.x - room->dimensions.x / 2, room->position.y, room->position.z - 4);
            room->door_list[i].rotation = vector3d(M_PI / 2, 0, 0);
            room->door_list[i].type = DOOR_NEG_X;
            break;
        case DOOR_POS_X:
            room->door_list[i].center = vector3d(room->position.x + room->dimensions.x / 2, room->position.y, room->position.z - 4);
            room->door_list[i].rotation = vector3d(M_PI / 2, 0, 0);
            room->door_list[i].type = DOOR_POS_X;
            break;
        case DOOR_NEG_Y:
            room->door_list[i].center = vector3d(room->position.x, room->position.y - room->dimensions.y / 2, room->position.z - 4);
            room->door_list[i].rotation = vector3d(0, 0, 0);
            room->door_list[i].type = DOOR_NEG_Y;
            break;
        case DOOR_POS_Y:
            room->door_list[i].center = vector3d(room->position.x, room->position.y + room->dimensions.y / 2, room->position.z - 4);
            room->door_list[i].rotation = vector3d(0, 0, 0);
            room->door_list[i].type = DOOR_POS_Y;
            break;

        case DOOR_UNDEFINED:
            slog("Door type undefined, expect a crash");
            break;
    }

    //slog("Center: x:%f, y:%f, z:%f", room->door_list[i].center.x, room->door_list[i].center.y, room->door_list[i].center.z);

    return &room->door_list[i];
}

void room_draw_doors(Uint32 bufferFrame, VkCommandBuffer commandBuffer, Room* room)
{
    if (!room)
    {
        slog("No room given");
        return;
    }

    for (int i = 0; i < room->max_doors; i++)
    {
        if (!room->modelMat) return;


        if (!room->door_list[i].model) continue;

        gfc_matrix_identity(room->door_list[i].modelMat);

        gfc_matrix_make_translation(room->door_list[i].modelMat, room->door_list[i].center);
        gfc_matrix_rotate(room->door_list[i].modelMat, room->door_list[i].modelMat, room->door_list[i].rotation.x, vector3d(0, 0, 1));

        gf3d_model_draw(room->door_list[i].model, bufferFrame, commandBuffer, room->door_list[i].modelMat);

    }
}

void room_setup_doors(Room* room)
{
    if (!room)
    {
        slog("No room given");
        return;
    }
    if (!room->door_list)
    {
        slog("No door list for room");
        return;
    }

    room_new_door(DOOR_NEG_X, room);
    room_new_door(DOOR_POS_X, room);
    room_new_door(DOOR_NEG_Y, room);
    room_new_door(DOOR_POS_Y, room);
}

void door_set_connected(DoorType type, Room* room)
{
    for (int i = 0; i < room->max_doors; i++)
    {
        if (room->door_list[i].type == type)
        {
            room->door_list[i].connected = true;
            //slog("Connected");
            break;
        }
    }

}
