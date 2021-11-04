#include "simple_logger.h"

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
        return NULL;
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
        return NULL;
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
        gfc_matrix_identity(game_world.room_list[i].modelMat);

        return &game_world.room_list[i];

    }
    slog("No free rooms in the world");
    return NULL;
}

void world_layout_rooms()
{
    
}

World world_get()
{
    return game_world;
}
