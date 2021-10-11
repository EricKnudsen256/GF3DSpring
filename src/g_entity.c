#include "simple_logger.h"

#include "g_entity.h"

EntityManager entity_manager = {0};


void entity_manager_init(Uint32 max_entities)
{
    if (!max_entities)
    {
        slog("Cannot allocate array of 0 entities!");
        return;
    }
    if (entity_manager.entity_list != NULL)
    {
        entity_manager_free();
    }
    
    entity_manager = gfc_allocate_array(sizeof(Entity), max_entities);
    
    if (!entity_manager.entity_list)
    {
        slog("Entity list not created!");
        return;
    }
    slog("Entity manager initialized");
}

void entity_manager_think()
{
    
}

void entity_manager_update()
{
    
}

void entity_manager_draw()
{
    
}

void entity_manager_free()
{
    
}

Entity* entity_new()
{
    if (!entity_manager.entity_list)
    {
        slog("Entity list not created, call entity_manager_init first");
        return;
    }
    for (int i = 0; i < entity_manager.max_entities; i++)
    {
        if(entity_manager.entity_list[i]._inuse)continue;
        
        entity_manager.entity_list[i]._inuse = true;
        return &entity_manager.entity_list[i];
        
    }
}

void entity_free(Entity *self)
{
    
}
