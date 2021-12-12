#include "simple_logger.h"

#include "o_object.h"

ObjectManager object_manager = { 0 };


void object_manager_init(Uint32 max_objects)
{
    if (!max_objects)
    {
        slog("Cannot allocate array of 0 objects!");
        return;
    }
    if (object_manager.object_list != NULL)
    {
        object_manager_free();
    }

    object_manager.object_list = gfc_allocate_array(sizeof(Object), max_objects);
    object_manager.max_objects = max_objects;

    if (!object_manager.object_list)
    {
        slog("Object list not created!");
        return;
    }
    slog("Object manager initialized");
    atexit(object_manager_free);
}


void entity_manager_draw()
{
    if (!object_manager.object_list)
    {
        slog("Object list not created, call object_manager_init first");
        return;
    }
    for (int i = 0; i < object_manager.max_objects; i++)
    {
        if (!object_manager.object_list[i])continue;

        object_draw(object_manager.object_list[i]);
    }
}


void object_manager_draw_hitboxes()
{
    if (!object_manager.object_list)
    {
        slog("Object list not created, call object_manager_init first");
        return;
    }
    for (int i = 0; i < object_manager.max_objects; i++)
    {
        if (!object_manager.object_list[i])continue;
        if (object_manager.object_list[i]->hitbox)
        {
            hitbox_draw(object_manager.object_list[i]->hitbox, object_manager.object_list[i]->modelMat);
        }
        if (object_manager.object_list[i]->interactBox)
        {
            hitbox_draw(object_manager.object_list[i]->interactBox, object_manager.object_list[i]->modelMat);
        }

    }
}


void object_manager_free()
{
    if (object_manager.object_list)
    {
        free(object_manager.object_list);
    }
    memset(&object_manager, 0, sizeof(ObjectManager));
    slog("Object system closed");
}

Object* object_new()
{
    if (!object_manager.object_list)
    {
        slog("Object list not created, call object_manager_init first");
        return NULL;
    }
    for (int i = 0; i < object_manager.max_objects; i++)
    {
        if (object_manager.object_list[i])continue;

        memset(object_manager.object_list[i], 0, sizeof(Object));
        object_manager.object_list[i]->_id = i;
        return object_manager.object_list[i];

    }
    slog("No free objects");
    return NULL;
}

void object_draw(Object* obj)
{
    if (!obj)
    {
        slog("No object given");
        return;
    }

    if (!obj->model) return;
    if (!obj->modelMat) return;

    gf3d_model_draw(obj->model, obj->modelMat, 0);
}

void object_free(Object* obj)
{
    if (!obj)
    {
        slog("No object given");
        return;
    }
    memset(object_manager.object_list[obj->_id], 0, sizeof(Object));
}
