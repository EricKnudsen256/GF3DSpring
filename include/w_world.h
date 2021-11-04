#ifndef __WORLD_H__
#define __WORLD_H__

#include "w_room.h"

typedef struct
{
    Room *room_list; 
    Uint32 max_rooms;
    
}World;

void world_init(Uint32 max_rooms);

void world_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer);

void world_draw_hitboxes(Uint32 bufferFrame, VkCommandBuffer commandBuffer);

void world_free();

Room* world_new_room();

void world_layout_rooms();

World world_get();


#endif
