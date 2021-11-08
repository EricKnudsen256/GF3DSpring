#include "simple_logger.h"

#include "g_random.h"

#include "w_world.h"

World game_world = {0};

void world_init(Uint32 max_rooms)
{
    game_world.room_list = gfc_allocate_array(sizeof(Room), max_rooms);
    game_world.max_rooms = max_rooms;

}

void world_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
    if (!game_world.room_list)
    {
        slog("World not initialized");
        return;
    }
    for (int i = 0; i < game_world.max_rooms; i++)
    {
        if (game_world.room_list[i]._inuse)
        {
            room_draw(bufferFrame, commandBuffer, &game_world.room_list[i]);
        }

    }
}

void world_draw_hitboxes(Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
    if (!game_world.room_list)
    {
        slog("World not initialized");
        return;
    }
    for (int i = 0; i < game_world.max_rooms; i++)
    {
        if (game_world.room_list[i]._inuse)
        {
            for (int j = 0; j < game_world.room_list[i].hitbox_max; j++)
            {
                //slog("test");
                if (game_world.room_list[i].hitbox_list[j])
                {
                    hitbox_draw(game_world.room_list[i].hitbox_list[j], bufferFrame, commandBuffer, game_world.room_list[i].modelMat);
                }
            }
        }
    }
}

void world_free()
{
    if (!game_world.room_list)
    {
        slog("World not initialized");
        return;
    }
    for (int i = 0; i < game_world.max_rooms; i++)
    {
       memset(&game_world.room_list[i], 0, sizeof(Room));
    }
}

Room* world_new_room()
{
    if (!game_world.room_list)
    {
        slog("World not initialized");
        return NULL;
    }
    for (int i = 0; i < game_world.max_rooms; i++)
    {
        if (game_world.room_list[i]._inuse)continue;

        memset(&game_world.room_list[i], 0, sizeof(Room));
        game_world.room_list[i]._inuse = true;
        game_world.room_list[i]._id = i;
        game_world.room_list[i].dimensions = vector3d(33.725, 33.725, 33.725);
        gfc_matrix_identity(game_world.room_list[i].modelMat);

        //NOTE: currently hard coding this value, change if more/less doors are wanted
        game_world.room_list[i].door_list = gfc_allocate_array(sizeof(Door), 4);
        game_world.room_list[i].max_doors = 4;

        //room_setup_doors(&game_world.room_list[i]);

        game_world.totalRooms++;
        return &game_world.room_list[i];

    }
    slog("No free rooms in the world");
    return NULL;
}

void world_layout_rooms()
{
    Room* startingRoom = world_new_room();
    startingRoom->model = gf3d_model_load("room1");
    room_make_hitboxs(startingRoom);
    room_set_position(vector3d(0, 0, 0), startingRoom);
    room_setup_doors(startingRoom);


    while (game_world.totalRooms < game_world.max_rooms - 20)
    {
        //total += 1;
        //slog("Total:%i", total);
        for (int i = 0; i < game_world.max_rooms; i++)
        {
            if (!game_world.room_list[i]._inuse)continue;

            float placeDoor = g_random();
            int randomDoor;

            if (placeDoor < .5)
            {
                //slog("r:%f", placeDoor);
                //place new room
                randomDoor = random_int_range(0, 3);
                DoorType type;

                switch (randomDoor)
                {
                case 0:
                    type = DOOR_NEG_X;
                    break;
                case 1:
                    type = DOOR_POS_X;
                    break;
                case 2:
                    type = DOOR_NEG_Y;
                    break;
                case 3:
                    type = DOOR_POS_Y;
                    break;
                }


                for (int door = 0; door < game_world.room_list[i].max_doors; door++)
                {
                    if (game_world.room_list[i].door_list[door].type == type && game_world.room_list[i].door_list[door].connected == false)
                    {
                        //slog("type:%i", type);
                        Vector3D newPos;
                        switch (type)
                        {
                        case DOOR_NEG_X:
                            newPos = vector3d(game_world.room_list[i].position.x - game_world.room_list[i].dimensions.x, game_world.room_list[i].position.y, game_world.room_list[i].position.z);
                            break;
                        case DOOR_POS_X:
                            newPos = vector3d(game_world.room_list[i].position.x + game_world.room_list[i].dimensions.x, game_world.room_list[i].position.y, game_world.room_list[i].position.z);
                            break;
                        case DOOR_NEG_Y:
                            newPos = vector3d(game_world.room_list[i].position.x, game_world.room_list[i].position.y - game_world.room_list[i].dimensions.y, game_world.room_list[i].position.z);
                            break;
                        case DOOR_POS_Y:
                            newPos = vector3d(game_world.room_list[i].position.x, game_world.room_list[i].position.y + game_world.room_list[i].dimensions.y, game_world.room_list[i].position.z);
                            break;

                        case DOOR_UNDEFINED:
                            slog("Door type undefined, expect a crash");
                            break;
                        }




                        if (world_check_for_room(newPos) == false)
                        {
                            Room* newRoom = world_new_room();

                            if (!newRoom)
                            {
                                break;
                            }

                            newRoom->model = gf3d_model_load("room1");
                            room_make_hitboxs(newRoom);
                            room_set_position(newPos, newRoom);
                            room_setup_doors(newRoom);

                            DoorType type2;

                            switch (type)
                            {
                            case DOOR_NEG_X:
                                type2 = DOOR_POS_X;
                                break;
                            case DOOR_POS_X:
                                type2 = DOOR_NEG_X;
                                break;
                            case DOOR_NEG_Y:
                                type2 = DOOR_POS_Y;
                                break;
                            case DOOR_POS_Y:
                                type2 = DOOR_NEG_Y;
                                break;

                            case DOOR_UNDEFINED:
                                slog("Door type undefined, expect a crash");
                                break;
                            }

                            door_set_connected(type2, newRoom);
                            door_set_connected(type, &game_world.room_list[i]);


                            break;
                        }
                    }
                }

            }
        }
        
    }



    for (int i = 0; i < game_world.max_rooms; i++)
    {
        Vector3D testPos;

        if (!game_world.room_list[i]._inuse) continue;

        //check all sides to see if room exists

        

        for (int door = 0; door < game_world.room_list[i].max_doors; door++)
        {
            
            if (game_world.room_list[i].door_list[door].connected == false)
            { 
                game_world.room_list[i].door_list[door].model = gf3d_model_load("door");
               
               
                        slog("Type:%i", game_world.room_list[i].door_list[door].type);
                        switch (game_world.room_list[i].door_list[door].type)
                        {
                            
                            case DOOR_NEG_X:
                                game_world.room_list[i].hitbox_list[5] = hitbox_new(game_world.room_list[i].door_list[door].center,
                                    vector3d(1, game_world.room_list[i].dimensions.y - 8, 20),
                                    vector3d(0, 0, 0));

                                break;

                            case DOOR_POS_X:
                                game_world.room_list[i].hitbox_list[6] = hitbox_new(game_world.room_list[i].door_list[door].center,
                                    vector3d(1, game_world.room_list[i].dimensions.y - 8, 20),
                                    vector3d(0, 0, 0));
                                break;

                            case DOOR_NEG_Y:
                                game_world.room_list[i].hitbox_list[7] = hitbox_new(game_world.room_list[i].door_list[door].center,
                                    vector3d(game_world.room_list[i].dimensions.x - 8, 1, 20),
                                    vector3d(0, 0, 0));
                                break;

                            case DOOR_POS_Y:
                                game_world.room_list[i].hitbox_list[8] = hitbox_new(game_world.room_list[i].door_list[door].center,
                                    vector3d(game_world.room_list[i].dimensions.x - 8, 1, 20),
                                    vector3d(0, 0, 0));
                                break;

                            case DOOR_UNDEFINED:
                                slog("Door type undefined, expect a crash");
                                break;

                                

                        }
            }
            
        }

    }

    

}

Bool world_check_for_room(Vector3D pos)
{
    float xMin, xMax;
    float yMin, yMax;
    Vector3D testPos;

    for (int i = 0; i < game_world.max_rooms; i++)
    {
        if (!game_world.room_list[i]._inuse)continue;


        xMin = game_world.room_list[i].position.x - game_world.room_list[i].dimensions.x / 2;
        xMax = game_world.room_list[i].position.x + game_world.room_list[i].dimensions.x / 2;

        yMin = game_world.room_list[i].position.y - game_world.room_list[i].dimensions.y / 2;
        yMax = game_world.room_list[i].position.y + game_world.room_list[i].dimensions.y / 2;

        testPos = pos;

        if (testPos.x > xMin && testPos.x < xMax && testPos.y > yMin && testPos.y < yMax)
        {
            /*
            slog("xMIN:%f, xMax:%f, yMin:%f, yMax:%f", xMin, xMax, yMin, yMax);
            slog("Pos: x:%f, y:%f, z:%f", testPos.x, testPos.y, testPos.z);
            slog("Return:%i", true);
            */
            return true;
        }
    }
    /*
    slog("xMIN:%f, xMax:%f, yMin:%f, yMax:%f", xMin, xMax, yMin, yMax);
    slog("Pos: x:%f, y:%f, z:%f", testPos.x, testPos.y, testPos.z);
    slog("Return:%i", false);
    */

    return false;
}

World world_get()
{
    return game_world;
}
