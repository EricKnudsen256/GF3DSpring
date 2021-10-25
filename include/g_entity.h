#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gf3d_model.h"
#include "gfc_types.h"

#include "g_hitbox.h"

typedef struct Entity_s
{
    Uint8 _inuse;
    Uint32 _id;
    Model *model;
    Vector3D position;
    Vector3D rotation;
    Vector3D velocity;
    Vector3D scale;
    Matrix4 modelMat;

    Hitbox *hitbox;
    
    void(*update)(struct Entity_s *self);
	void(*think)(struct Entity_s *self);
	void(*draw)(Uint32 bufferFrame, VkCommandBuffer commandBuffer, struct Entity_s *self);
    
}Entity;

typedef struct 
{
    Entity *entity_list;
    Uint32 max_entities;
}EntityManager;

void entity_manager_init(Uint32 max_entities);

void entity_manager_think();

void entity_manager_update();

void entity_manager_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer);

void entity_manager_free();

Entity* entity_new();

void entity_make_hitbox(Vector3D dimensions, Entity* self);

void entity_think(Entity *self);

void entity_update(Entity* self);

void entity_draw(Uint32 bufferFrame, VkCommandBuffer commandBuffer, Entity* self);

void entity_free(Entity *self);

#endif
