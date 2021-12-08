#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gf3d_model.h"
#include "gfc_types.h"

#include "g_hitbox.h"

typedef enum 
{
    ENT_UNDEFINED,
    ENT_PLAYER,
    ENT_MONSTER

}EntType;

typedef struct Entity_s
{
    Uint8 _inuse;           /**boolean flag to check if an entity is used in the entity list */
    Uint32 _id;             /**interger ID of entity in the entity manager list */
    Model *model;           /**model to draw for the entity. Not required */
    Vector3D position;      /**xyz position of the entity in the world.  */
    Vector3D rotation;      /**yaw, roll, pitch values for the entity */
    Vector3D velocity;      /**xyz velocities for the entity */
    Vector3D scale;         /**xyz scale values for the entity */
    Matrix4 modelMat;       /**4d matrix for displaying the entity */

    Vector3D forward, back; /**forward and backwards vectors for the entity populated every frame */
    Vector3D left, right;   /**left and right vectors for the entity populated every frame */
    Vector3D up, down;      /**up and down vectors for the entity populated every frame */

    Hitbox *hitbox;         /**rectangular hitbox for entity collision */

    Uint32 health;

    Bool dead;

    EntType type;
    
    void(*update)(struct Entity_s* self);   /**void function to update entity values and position */
	void(*think)(struct Entity_s *self);    /**void function for entity logic */
	void(*draw)(struct Entity_s *self);  /**draw function for the entity */
    void(*free)(struct Entity_s* self);  
    void(*kill)(struct Entity_s* self);  /**draw function for the entity */

    void* parent;           /**void pointer to the parent object that created this entity */
    
}Entity;

typedef struct 
{
    Entity *entity_list;    /**list of all entities in the game */
    Uint32 max_entities;    /**maximum number of possible entities defined in game.c */
}EntityManager;

/**
* 
*/

/**
* @brief initializes the entity system
* @params max_entities the maximum number of entities to support
*/
void entity_manager_init(Uint32 max_entities);

/**
* @brief calls the think function on all current entities
*/
void entity_manager_think();

/**
* @brief calls the update function on all current entities
*/
void entity_manager_update();

/**
* @brief calls the draw function on all current entities
* @params bufferFrame the currently active bufferframe to draw to
* @params commandBuffer the command buffer to write the draw commands to
*/
void entity_manager_draw();

/**
* @brief if the entity has a hitbox, will call the relevant draw command to show it
* @params bufferFrame the currently active bufferframe to draw to
* @params commandBuffer the command buffer to write the draw commands to
*/
void entity_manager_draw_hitboxes();

/**
* @brief checks collions on all entities in the entity manager
* @param ent the entity to check for collisions
*/
void entity_manager_check_collions();

Entity* entity_manager_get_player();

/**
* @brief frees the entity manager subsystem
*/
void entity_manager_free();

/**
* @brief allocates a new entity in the entity list, and sets a few default values
* @return the newly created entity
*/
Entity* entity_new();

/**
* @brief creates a hitbox for the selected entity
* @params dimensions the wdh of the created hitbox
* @params offset the xyz of the offset from the center of the entity
* @params self the entity to create the hitbox for
*/
void entity_make_hitbox(Vector3D dimensions, Vector3D offset, Entity* self);


/**
* @brief called for basic entity log
* @params self the entity to think
*/
void entity_think(Entity *self);

/**
* @brief called for updating entity values
* @params self the entity to update
*/
void entity_update(Entity* self);

/**
* @brief draws the provided entity
* @params bufferFrame the currently active bufferframe to draw to
* @params commandBuffer the command buffer to write the draw commands to
* @params self the entity to update
*/
void entity_draw(Entity* self);

/**
* @brief checks collions on entities against all tiles, adjusts
* @param ent the entity to check for collisions
*/
void entity_check_collisions(Entity* self);

/**
* @brief frees the memory of the entity in the entity list
* @params self the entity to free
*/
void entity_free(Entity *self);

#endif
