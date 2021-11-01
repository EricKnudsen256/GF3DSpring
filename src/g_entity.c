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
    
    entity_manager.entity_list = gfc_allocate_array(sizeof(Entity), max_entities);
    entity_manager.max_entities = max_entities;
    
    if (!entity_manager.entity_list)
    {
        slog("Entity list not created!");
        return;
    }
    slog("Entity manager initialized");
    atexit(entity_manager_free);
}

void entity_manager_think()
{
    if (!entity_manager.entity_list)
    {
        slog("Entity list not created, call entity_manager_init first");
        return;
    }
    for (int i = 0; i < entity_manager.max_entities; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;

        entity_manager.entity_list[i].think(&entity_manager.entity_list[i]);

    }
}

void entity_manager_update()
{
    if (!entity_manager.entity_list)
    {
        slog("Entity list not created, call entity_manager_init first");
        return;
    }
    for (int i = 0; i < entity_manager.max_entities; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;

        entity_manager.entity_list[i].update(&entity_manager.entity_list[i]);

    }
}

void entity_manager_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
    if (!entity_manager.entity_list)
    {
        slog("Entity list not created, call entity_manager_init first");
        return;
    }
    for (int i = 0; i < entity_manager.max_entities; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;

        entity_manager.entity_list[i].draw(bufferFrame, commandBuffer, &entity_manager.entity_list[i]);

    }
}

void entity_manager_draw_hitboxes(Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
    if (!entity_manager.entity_list)
    {
        slog("Entity list not created, call entity_manager_init first");
        return;
    }
    for (int i = 0; i < entity_manager.max_entities; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        if (!entity_manager.entity_list[i].hitbox)continue;

        hitbox_draw(entity_manager.entity_list[i].hitbox, bufferFrame, commandBuffer, entity_manager.entity_list[i].modelMat);

    }
}

void entity_manager_free()
{
    if (entity_manager.entity_list)
    {
        free(entity_manager.entity_list);
    }
    memset(&entity_manager, 0, sizeof(EntityManager));
    slog("Entity system closed");

}

Entity* entity_new()
{
    if (!entity_manager.entity_list)
    {
        slog("Entity list not created, call entity_manager_init first");
        return NULL;
    }
    for (int i = 0; i < entity_manager.max_entities; i++)
    {
        if(entity_manager.entity_list[i]._inuse)continue;
        
        memset(&entity_manager.entity_list[i], 0, sizeof(Entity));
        entity_manager.entity_list[i]._inuse = true;
        entity_manager.entity_list[i]._id = i;
        entity_manager.entity_list[i].think = entity_think;
        entity_manager.entity_list[i].update = entity_update;
        entity_manager.entity_list[i].draw = entity_draw;
        return &entity_manager.entity_list[i];
        
    }
    slog("No free entities");
    return NULL;
}

void entity_make_hitbox(Vector3D dimensions, Vector3D offset, Entity* self)
{
    if (!self) return;
    if (self->hitbox)
    {
        self->hitbox = NULL;
    }

    self->hitbox = hitbox_new(vector3d(0,0,0), dimensions, offset);
}

void entity_think(Entity* self)
{

}

void entity_update(Entity* self)
{
    gfc_matrix_identity(self->modelMat);
    
    gfc_matrix_make_translation(
        self->modelMat,
            self->position
        );
    
    if(self->hitbox)
    {
        hitbox_set_pos(self->position, self->hitbox);
    }
}

void entity_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer, Entity* self)
{
    if (!self)
    {
        slog("No entity given");
        return;
    }

    if (!self->model) return;
    if (!self->modelMat) return;

    gf3d_model_draw(self->model, bufferFrame, commandBuffer, self->modelMat);

}

void entity_free(Entity *self)
{
    if (!self)
    {
        slog("No entity given");
        return;
    }
}
