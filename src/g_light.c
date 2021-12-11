#include "simple_logger.h"

#include "g_light.h"

LightingManager lighting_manager = { 0 };


void lighting_manager_init(Uint32 max_lights)
{
    if (!max_lights)
    {
        slog("Cannot allocate array of 0 light!");
        return;
    }
    if (lighting_manager.light_list != NULL)
    {
       lighting_manager_free();
    }

    lighting_manager.light_list = gfc_allocate_array(sizeof(Light), max_lights);
    lighting_manager.max_lights = max_lights;

    if (!lighting_manager.light_list)
    {
        slog("Lighting list not created!");
        return;
    }
    slog("Lighting manager initialized");
    atexit(lighting_manager_free);
}


Light *lighting_manager_get_closest_light(Vector3D position)
{
    int closestDistance = 100;
    int lightDistance;
    Light *closestLight = NULL;

    for (int i = 0; i < lighting_manager.max_lights; i++)
    {
        /*
        lightDistance = pow(position.x - lighting_manager.light_list[i].pos.x, 2) + pow(position.y - lighting_manager.light_list[i].pos.y, 2) + pow(position.z - lighting_manager.light_list[i].pos.z, 2);

        if (lightDistance < pow(closestDistance, 2))
        {
            closestDistance = sqrt(lightDistance);
            closestLight = &lighting_manager.light_list[i];
        }
        */
        if (lighting_manager.light_list[i]._inuse)
        {
            return &lighting_manager.light_list[i];
        }
    }

    if (closestLight == NULL)
    {
        slog("Closest Light is null, is the function working properly?");
    }

    return closestLight;
}


void lighting_manager_free()
{
    if (lighting_manager.light_list)
    {
        free(lighting_manager.light_list);
    }
    memset(&lighting_manager, 0, sizeof(LightingManager));
    slog("Lighting system closed");
}


Light* light_new(Vector3D position)
{
    if (!lighting_manager.light_list)
    {
        slog("Light list not created, call llighting_manager_init first");
        return NULL;
    }
    for (int i = 0; i < lighting_manager.max_lights; i++)
    {
        if (lighting_manager.light_list[i]._inuse)continue;

        memset(&lighting_manager.light_list[i], 0, sizeof(Light));
        lighting_manager.light_list[i]._inuse = true;
        lighting_manager.light_list[i]._id = i;
        return &lighting_manager.light_list[i];

    }
    slog("No free Lights");
    return NULL;
}


void light_set_pos(Vector3D position, Light* light)
{
    if (!light)
    {
        slog("No light given");
        return;
    }

    light->pos = position;
}


void light_set_color(Vector4D color, Light* light)
{
    if (!light)
    {
        slog("No light given");
        return;
    }

    light->lightColor = color;
}

void light_set_ambient_color(Vector4D color, Light* light)
{
    if (!light)
    {
        slog("No light given");
        return;
    }

    light->ambientLightColor = color;
}

LightUBO light_create_ubo_from_light(Light* light)
{
    if (!light)
    {
        slog("No light given");
        return;
    }

    LightUBO ubo;

    ubo.pos = vector4d(light->pos.x, light->pos.y, light->pos.z, 0);
    ubo.ambientLightColor = light->ambientLightColor;
    ubo.lightColor = light->lightColor;

    return ubo;

}


void light_free(Light* light)
{
    if (!light)
    {
        slog("No light given");
        return;
    }
    memset(&lighting_manager.light_list[light->_id], 0, sizeof(Light));
}




