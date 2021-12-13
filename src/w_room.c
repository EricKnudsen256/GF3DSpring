#include "simple_logger.h"
#include "simple_json.h"

#include "g_random.h"

#include "o_object.h"
#include "o_bed.h"
#include "o_locker.h"
#include "o_lever.h"
#include "o_vent.h"

#include "w_room.h"


void room_draw(Room *room)
{
    if (!room)
    {
        slog("No room given");
        return;
    }

    if (!room->model) return;
    if (!room->modelMat) return;

    gf3d_model_draw(room->model, room->modelMat, 0);
    room_draw_doors(room);
}

void room_make_hitboxs(Room* room)
{
    if (!room) return;
    if (room->hitbox_list)
    {
        room->hitbox_list = NULL;
    }

    room->hitbox_list = gfc_allocate_array(sizeof(Hitbox), 10);
    room->hitbox_max = 20;

    //test room hitbox to see if scale is right
    room->hitbox_list[0] = hitbox_new(vector3d(0, 0, 0), vector3d(60, 60, 1), vector3d(0, 0, -60/2));
    room->hitbox_list[0]->type = HITBOX_WORLD;
    room->hitbox_list[0]->parent = room;

    room->hitbox_list[1] = hitbox_new(vector3d(0, 0, 0), vector3d(20, .5, 60), vector3d(60 / 2 - 10, 60 / 2 - 0, 0));
    room->hitbox_list[1]->type = HITBOX_WORLD;
    room->hitbox_list[1]->parent = room;

    room->hitbox_list[2] = hitbox_new(vector3d(0, 0, 0), vector3d(20, .5, 60), vector3d(-(60 / 2 - 10), 60 / 2 - 0, 0));
    room->hitbox_list[2]->type = HITBOX_WORLD;
    room->hitbox_list[2]->parent = room;

    room->hitbox_list[3] = hitbox_new(vector3d(0, 0, 0), vector3d(20, .5, 60), vector3d(-(60 / 2 - 10), -(60 / 2 - 0), 0));
    room->hitbox_list[3]->type = HITBOX_WORLD;
    room->hitbox_list[3]->parent = room;

    room->hitbox_list[4] = hitbox_new(vector3d(0, 0, 0), vector3d(20, .5, 60), vector3d(60 / 2 - 10, -(60 / 2 - 0), 0));
    room->hitbox_list[4]->type = HITBOX_WORLD;
    room->hitbox_list[4]->parent = room;



    room->hitbox_list[11] = hitbox_new(vector3d(0, 0, 0), vector3d(.5, 20, 60), vector3d(60 / 2 - 0, 60 / 2 - 10, 0));
    room->hitbox_list[11]->type = HITBOX_WORLD;
    room->hitbox_list[11]->parent = room;

    room->hitbox_list[12] = hitbox_new(vector3d(0, 0, 0), vector3d(.5, 20, 60), vector3d(-(60 / 2 - 0), 60 / 2 - 10, 0));
    room->hitbox_list[12]->type = HITBOX_WORLD;
    room->hitbox_list[12]->parent = room;

    room->hitbox_list[13] = hitbox_new(vector3d(0, 0, 0), vector3d(.5, 20, 60), vector3d(-(60 / 2 - 0), -(60 / 2 - 10), 0));
    room->hitbox_list[13]->type = HITBOX_WORLD;
    room->hitbox_list[13]->parent = room;

    room->hitbox_list[14] = hitbox_new(vector3d(0, 0, 0), vector3d(.5, 20, 60), vector3d(60 / 2 - 0, -(60 / 2 - 10), 0));
    room->hitbox_list[14]->type = HITBOX_WORLD;
    room->hitbox_list[14]->parent = room;
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
            room->door_list[i].center = vector3d(room->position.x - room->dimensions.x / 2, room->position.y, room->position.z - 18);
            room->door_list[i].rotation = vector3d(M_PI / 2, 0, 0);
            room->door_list[i].type = DOOR_NEG_X;
            break;
        case DOOR_POS_X:
            room->door_list[i].center = vector3d(room->position.x + room->dimensions.x / 2, room->position.y, room->position.z - 18);
            room->door_list[i].rotation = vector3d(M_PI / 2, 0, 0);
            room->door_list[i].type = DOOR_POS_X;
            break;
        case DOOR_NEG_Y:
            room->door_list[i].center = vector3d(room->position.x, room->position.y - room->dimensions.y / 2, room->position.z - 18);
            room->door_list[i].rotation = vector3d(0, 0, 0);
            room->door_list[i].type = DOOR_NEG_Y;
            break;
        case DOOR_POS_Y:
            room->door_list[i].center = vector3d(room->position.x, room->position.y + room->dimensions.y / 2, room->position.z - 18);
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

void room_draw_doors(Room* room)
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
        if (room->door_list[i].hitbox->type == HITBOX_INACTIVE) continue;

        gfc_matrix_identity(room->door_list[i].modelMat);

        gfc_matrix_make_translation(room->door_list[i].modelMat, room->door_list[i].center);
        gfc_matrix_rotate(room->door_list[i].modelMat, room->door_list[i].modelMat, room->door_list[i].rotation.x, vector3d(0, 0, 1));

        gf3d_model_draw(room->door_list[i].model, room->door_list[i].modelMat, 0);

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

void room_load_layout_random(Room* room)
{
    SJson* json, * jRoom, * toRead, * testJson, * objJson;
    SJson* array1, * array2;

    TextLine filename;

    int i = 0;

    snprintf(filename, GFCLINELEN, "template/room%i.template", i);

    testJson = sj_load(filename);

    while (testJson)
    {
        i++;
        snprintf(filename, GFCLINELEN, "template/room%i.template", i);
        testJson = sj_load(filename);
    }

    int randomLayout = random_int_range(0, i);

    snprintf(filename, GFCLINELEN, "template/room%i.template", randomLayout);
    json = sj_load(filename);

    if (!json)
    {
        slog("Problem loading template file, not sure what occured");
        return;
    }

    jRoom = sj_object_get_value(json, "room");

    for (int objCount = 0; objCount < sj_array_get_count(jRoom); objCount++)
    {
        Bool leverspawned = false;

        Object* spawned;

        toRead = sj_array_get_nth(jRoom, objCount);
        int objType;

        sj_get_integer_value(sj_object_get_value(toRead, "type"), &objType);
        Vector3D position = vector3d(0, 0, 0);
        Vector3D rotation = vector3d(0, 0, 0);

        sj_get_float_value(sj_object_get_value(toRead, "positionX"), &position.x);
        sj_get_float_value(sj_object_get_value(toRead, "positionY"), &position.y);
        sj_get_float_value(sj_object_get_value(toRead, "positionZ"), &position.z);

        sj_get_float_value(sj_object_get_value(toRead, "rotationX"), &rotation.x);
        sj_get_float_value(sj_object_get_value(toRead, "rotationY"), &rotation.y);
        sj_get_float_value(sj_object_get_value(toRead, "rotationZ"), &rotation.z);

        position.x += room->position.x;
        position.y += room->position.y;
        position.z += room->position.z;

        switch (objType)
        {
            case OBJ_BED:
                spawned = bed_new(position, rotation);
                break;
            case OBJ_LOCKER:
                spawned = locker_new(position, rotation);
                break;
            case OBJ_LEVER:
                if (leverspawned)
                {
                    slog("Only one lever possible per room");
                    break;
                }
                spawned = lever_new(position, rotation, room);
                leverspawned = true;
                break;

            case OBJ_VENT:
                spawned = vent_new(position, rotation, room);
                break;
        }
    }
}

void editor_save_layout(Room* room)
{
    SJson* json, * jRoom, *toInsert, *testJson, *objJson;

    TextLine filename;

    Object* object_list = object_manager_get_object_list();
    int object_max = object_manager_get_object_max();

    int i = 0;

    snprintf(filename, GFCLINELEN, "template/room%i.template", i);

    testJson = sj_load(filename);

    while (testJson)
    {
        i++;
        snprintf(filename, GFCLINELEN, "template/room%i.template", i);
        testJson = sj_load(filename);
    }

    snprintf(filename, GFCLINELEN, "template/room%i.template", i);
    testJson = sj_load(filename);

    json = sj_object_new();

    jRoom = sj_array_new();

    for (int count = 0; count < object_max; count++)
    {
        if (object_list[count]._inuse)
        {
            //save json information here

            objJson = sj_object_new();

            toInsert = sj_new_int(object_list[count].type);
            sj_object_insert(objJson, "type", toInsert);

            toInsert = sj_new_float(object_list[count].position.x);
            sj_object_insert(objJson, "positionX", toInsert);

            toInsert = sj_new_float(object_list[count].position.y);
            sj_object_insert(objJson, "positionY", toInsert);

            toInsert = sj_new_float(object_list[count].position.z);
            sj_object_insert(objJson, "positionZ", toInsert);

            toInsert = sj_new_float(object_list[count].rotation.x);
            sj_object_insert(objJson, "rotationX", toInsert);
            
            toInsert = sj_new_float(object_list[count].rotation.y);
            sj_object_insert(objJson, "rotationY", toInsert);

            toInsert = sj_new_float(object_list[count].rotation.z);
            sj_object_insert(objJson, "rotationZ", toInsert);

            sj_array_append(jRoom, objJson);

        }
    }

    sj_object_insert(json, "room", jRoom);
    sj_save(json, filename); //use as last line to make sure this shit actually saves

    slog("Layout saved!");


}
