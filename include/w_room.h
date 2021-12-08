#ifndef __ROOM_H__
#define __ROOM_H__

#include "gf3d_model.h"
#include "gfc_types.h"
#include "gfc_vector.h"

#include "g_hitbox.h"

typedef enum
{
    DOOR_UNDEFINED,
    DOOR_NEG_X,
    DOOR_NEG_Y,
    DOOR_POS_X,
    DOOR_POS_Y
}DoorType;

typedef struct
{
    Bool _inuse;
    Vector3D center;
    DoorType type;
    Bool connected;
    Bool locked;

    Model* model;
    Matrix4 modelMat;

    Vector3D dimensions; //assuming this is a rectangular room
    Vector3D rotation;
    
}Door;

typedef struct
{
    Uint8 _inuse;
    Uint32 _id;
    Model *model;
    Vector3D position;
    Vector3D dimensions; //assuming this is a rectangular room
    Vector3D rotation;
    Vector3D scale;
    Matrix4 modelMat;

    Hitbox **hitbox_list;
    Uint32 hitbox_max;
    
    Door *door_list;
    Uint32 max_doors;
    
}Room;

void room_draw(Room *room);

void room_make_hitboxs(Room* room);

void room_set_position(Vector3D pos, Room *room);

Door* room_new_door(DoorType type, Room* room);

void room_draw_doors(Room* room);

void room_setup_doors(Room* room);

void door_set_connected(DoorType type, Room* room);

#endif
