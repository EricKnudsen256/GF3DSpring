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


void object_manager_draw()
{
    if (!object_manager.object_list)
    {
        slog("Object list not created, call object_manager_init first");
        return;
    }
    for (int i = 0; i < object_manager.max_objects; i++)
    {
        if (!object_manager.object_list[i]._inuse)continue;

        object_draw(&object_manager.object_list[i]);
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
        if (!object_manager.object_list[i]._inuse)continue;
        if (object_manager.object_list[i].hitbox)
        {
            hitbox_draw(object_manager.object_list[i].hitbox, object_manager.object_list[i].modelMat);
        }
        if (object_manager.object_list[i].interactBox)
        {
            hitbox_draw(object_manager.object_list[i].interactBox, object_manager.object_list[i].modelMat);
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

Object* object_manager_get_object_list()
{
    return object_manager.object_list;
}

Uint32 object_manager_get_object_max()
{
    return object_manager.max_objects;
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
        if (object_manager.object_list[i]._inuse)continue;

        memset(&object_manager.object_list[i], 0, sizeof(Object));

        object_manager.object_list[i]._id = i;
        object_manager.object_list[i]._inuse = 1;
        return &object_manager.object_list[i];

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

    gfc_matrix_identity(obj->modelMat);
    gfc_matrix_make_translation(obj->modelMat, obj->position);
    gfc_matrix_rotate(obj->modelMat, obj->modelMat, obj->rotation.x, vector3d(1, 0, 0));
    gfc_matrix_rotate(obj->modelMat, obj->modelMat, obj->rotation.z, vector3d(0, 0, 1));

    gf3d_model_draw(obj->model, obj->modelMat, 0);
}

void object_free(Object* obj)
{
    if (!obj)
    {
        slog("No object given");
        return;
    }
    obj->free(obj);
    memset(&object_manager.object_list[obj->_id], 0, sizeof(Object));
}
