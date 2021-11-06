#include "simple_logger.h"

#include "g_entity.h"
#include "g_time.h"

#include "w_world.h"

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
        if (!entity_manager.entity_list[i].think)continue;

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
        if (!entity_manager.entity_list[i].update)continue;

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
        if (!entity_manager.entity_list[i].draw)continue;

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

void entity_manager_check_collions()
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

        entity_check_collisions(&entity_manager.entity_list[i]);

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
    self->hitbox->parent = self;
}

void entity_think(Entity* self)
{
    //self->velocity.z -= .5 * get_delta_time();
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

void entity_check_collisions(Entity* self)
{
    Entity* ent;
    //go through list of entities, check for hitboxes

    if (!self)
    {
        slog("No ent to check collisions of");
        return;
    }
    if (!self->hitbox) return;

    for (int i = 0; i < entity_manager.max_entities; i++)
    {
        ent = &entity_manager.entity_list[i];
        if (!ent->_inuse)continue;
        if (ent->_id == self->_id) continue;

        if (!vector3d_distance_between_less_than(self->hitbox->center, ent->hitbox->center, 100)) continue;

        //check x collision
        if (hitbox_check_collision(self->hitbox, ent->hitbox, vector3d(self->velocity.x, 0, 0)))
        {
            //slog("Collision: true");
            if (self->velocity.x > 0)
            {
                //slog("Vel:x:%f", self->velocity.x);
                self->position.x = (ent->hitbox->center.x) - (ent->hitbox->dimensions.x / 2) - (self->hitbox->dimensions.x / 2);
                self->hitbox->center.x = (ent->hitbox->center.x) - (ent->hitbox->dimensions.x / 2) - (self->hitbox->dimensions.x / 2);

                self->velocity.x = 0;
            }
            else if (self->velocity.x < 0)
            {
                //slog("Vel:x:%f", self->velocity.x);
                self->position.x = (ent->hitbox->center.x) + (ent->hitbox->dimensions.x / 2) + (self->hitbox->dimensions.x / 2);
                self->hitbox->center.x = (ent->hitbox->center.x) + (ent->hitbox->dimensions.x / 2) + (self->hitbox->dimensions.x / 2);

                self->velocity.x = 0;
            }


        }

        //check y collision
        if (hitbox_check_collision(self->hitbox, ent->hitbox, vector3d(0, self->velocity.y, 0)))
        {

            if (self->velocity.y > 0)
            {
                //slog("Vel:x:%f", self->velocity.x);
                self->position.y = (ent->hitbox->center.y) - (ent->hitbox->dimensions.y / 2) - (self->hitbox->dimensions.y / 2);
                self->hitbox->center.y = (ent->hitbox->center.y) - (ent->hitbox->dimensions.y / 2) - (self->hitbox->dimensions.y / 2);

                self->velocity.y = 0;
            }
            else if (self->velocity.y < 0)
            {
                //slog("Vel:x:%f", self->velocity.x);
                self->position.y = (ent->hitbox->center.y) + (ent->hitbox->dimensions.y / 2) + (self->hitbox->dimensions.y / 2);
                self->hitbox->center.y = (ent->hitbox->center.y) + (ent->hitbox->dimensions.y / 2) + (self->hitbox->dimensions.y / 2);

                self->velocity.y = 0;
            }
        }

        //check z collision
        if (hitbox_check_collision(self->hitbox, ent->hitbox, vector3d(0, 0, self->velocity.z)))
        {
            if (self->velocity.z > 0)
            {
                //slog("Vel:x:%f", self->velocity.x);
                self->position.z = (ent->hitbox->center.z) - (ent->hitbox->dimensions.z / 2) - (self->hitbox->dimensions.z / 2);
                self->hitbox->center.z = (ent->hitbox->center.z) - (ent->hitbox->dimensions.z / 2) - (self->hitbox->dimensions.z / 2);

                self->velocity.z = 0;
            }
            else if (self->velocity.z < 0)
            {
                //slog("Vel:x:%f", self->velocity.x);
                self->position.z = (ent->hitbox->center.z) + (ent->hitbox->dimensions.z / 2) + (self->hitbox->dimensions.z / 2);
                self->hitbox->center.z = (ent->hitbox->center.z) + (ent->hitbox->dimensions.z / 2) + (self->hitbox->dimensions.z / 2);

                self->velocity.z = 0;
            }
        }
    }

    //then go through list of world objects to check for hitboxes

    World world = world_get();

    for (int i = 0; i < world.max_rooms; i++)
    {
        if (world.room_list[i]._inuse)
        {
            for (int j = 0; j < world.room_list[i].hitbox_max; j++)
            {
                //slog("test");
                if (world.room_list[i].hitbox_list[j])
                {
                    Hitbox* hitbox = world.room_list[i].hitbox_list[j];

                    if (!vector3d_distance_between_less_than(self->hitbox->center, hitbox->center, 100)) continue;

                    //check x collision
                    if (hitbox_check_collision(self->hitbox, hitbox, vector3d(self->velocity.x, 0, 0)))
                    {
                        //slog("Collision: true");
                        if (self->velocity.x > 0)
                        {
                            //slog("Vel:x:%f", self->velocity.x);
                            self->position.x = (hitbox->center.x) - (hitbox->dimensions.x / 2) - (self->hitbox->dimensions.x / 2);
                            self->hitbox->center.x = (hitbox->center.x) - (hitbox->dimensions.x / 2) - (self->hitbox->dimensions.x / 2);

                            self->velocity.x = 0;
                        }
                        else if (self->velocity.x < 0)
                        {
                            //slog("Vel:x:%f", self->velocity.x);
                            self->position.x = (hitbox->center.x) + (hitbox->dimensions.x / 2) + (self->hitbox->dimensions.x / 2);
                            self->hitbox->center.x = (hitbox->center.x) + (hitbox->dimensions.x / 2) + (self->hitbox->dimensions.x / 2);

                            self->velocity.x = 0;
                        }


                    }

                    //check y collision
                    if (hitbox_check_collision(self->hitbox, hitbox, vector3d(0, self->velocity.y, 0)))
                    {

                        if (self->velocity.y > 0)
                        {
                            //slog("Vel:x:%f", self->velocity.x);
                            self->position.y = (hitbox->center.y) - (hitbox->dimensions.y / 2) - (self->hitbox->dimensions.y / 2);
                            self->hitbox->center.y = (hitbox->center.y) - (hitbox->dimensions.y / 2) - (self->hitbox->dimensions.y / 2);

                            self->velocity.y = 0;
                        }
                        else if (self->velocity.y < 0)
                        {
                            //slog("Vel:x:%f", self->velocity.x);
                            self->position.y = (hitbox->center.y) + (hitbox->dimensions.y / 2) + (self->hitbox->dimensions.y / 2);
                            self->hitbox->center.y = (hitbox->center.y) + (hitbox->dimensions.y / 2) + (self->hitbox->dimensions.y / 2);

                            self->velocity.y = 0;
                        }
                    }

                    //check z collision
                    if (hitbox_check_collision(self->hitbox, hitbox, vector3d(0, 0, self->velocity.z)))
                    {
                        if (self->velocity.z > 0)
                        {
                            //slog("Vel:x:%f", self->velocity.x);
                            self->position.z = (hitbox->center.z) - (hitbox->dimensions.z / 2) - (self->hitbox->dimensions.z / 2);
                            self->hitbox->center.z = (hitbox->center.z) - (hitbox->dimensions.z / 2) - (self->hitbox->dimensions.z / 2);

                            self->velocity.z = 0;
                        }
                        else if (self->velocity.z < 0)
                        {
                            //slog("Vel:x:%f", self->velocity.x);
                            self->position.z = (hitbox->center.z) + (hitbox->dimensions.z / 2) + (self->hitbox->dimensions.z / 2);
                            self->hitbox->center.z = (hitbox->center.z) + (hitbox->dimensions.z / 2) + (self->hitbox->dimensions.z / 2);

                            self->velocity.z = 0;
                        }
                    }

                }
            }
        }
    }


}

void entity_free(Entity *self)
{
    if (!self)
    {
        slog("No entity given");
        return;
    }
}
