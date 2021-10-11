#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gf3d_model.h"

typedef struct Entity_s
{
    Uint8 _inuse;
    Uint32 _id;
    Model *model;
    Vector3 position;
    Vector3 rotation;
    Vector3 velocity;
    Vector3 scale;
    Matrix4 modelMat;
    
    void(*update)(struct Entity_s *self);
	void(*think)(struct Entity_s *self);
	void(*draw)(struct Entity_s *self);
    
}Entity;

typedef struct 
{
    Entity *entity_list;
    Uint32 max_entities;
}EntityManager;

void entity_manager_init(Uint32 max_entities);

void entity_manager_think();

void entity_manager_update();

void entity_manager_draw();

void entity_manager_free();

Entity* entity_new();

void entity_free(Entity *self);

#endif
