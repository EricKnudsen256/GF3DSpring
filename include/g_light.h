#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "gfc_matrix.h"
#include "gfc_vector.h"
#include "gfc_types.h"

typedef struct
{
    Vector4D pos;
    Vector4D lightColor;
    Vector4D ambientLightColor;

}LightUBO;


typedef struct
{
    Bool _inuse;
    int _id;

    Vector3D pos;
    Vector4D lightColor;
    Vector4D ambientLightColor;

}Light;

typedef struct
{
    Light* light_list;    /**list of all lights in the game */
    Uint32 max_lights;    /**maximum number of possible lights defined in game.c */
}LightingManager;



/**
* @brief initializes the lighting system
* @params max_lights the maximum number of lights to support
*/
void lighting_manager_init(Uint32 max_lights);

/**
* @brief returns the closest light to the given position
* @params position the position to check for lights
*/
Light *lighting_manager_get_closest_light(Vector3D position);


/**
* @brief frees the lighting manager subsystem
*/
void lighting_manager_free();

/**
* @brief creates a new light
* @params position the starting position of the light object
* @return the newly created light, NULL on error
*/
Light* light_new(Vector3D position);

/**
* @brief sets the position of the light
* @params position the new center position of the light
*/
void light_set_pos(Vector3D position, Light* light);

/**
* @brief sets the color
* @params color the RGB+Brightness value of the light
*/
void light_set_color(Vector4D color, Light* light);

void light_set_ambient_color(Vector4D color, Light* light);

LightUBO light_create_ubo_from_light(Light* light);

/**
* @brief frees memeory of created light
* @params light the light to free
*/
void light_free(Light* light);


#endif