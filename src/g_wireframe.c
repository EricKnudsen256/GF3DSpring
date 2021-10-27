#include <stddef.h>

#include "simple_logger.h"

#include "gf3d_vgraphics.h"
#include "g_wireframe.h"
#include "gf3d_obj_load.h"
#include "gf3d_swapchain.h"
#include "gf3d_commands.h"


#define ATTRIBUTE_COUNT 3


typedef struct
{
    Mesh* wireframe_list;
    Uint32 wireframe_max;
    VkVertexInputAttributeDescription attributeDescriptions[ATTRIBUTE_COUNT];
    VkVertexInputBindingDescription bindingDescription;
    Command* stagingCommandBuffer;
}WireframeSystem;

static WireframeSystem gf3d_wireframe = { 0 };

void gf3d_wireframe_close();
void gf3d_wireframe_delete(Wireframe* wireframe);

void gf3d_wireframe_init(Uint32 wireframe_max)
{
    if (!wireframe_max)
    {
        slog("failed to initialize wireframe system: cannot allocate 0 mesh_max");
        return;
    }
    atexit(gf3d_wireframe_close);
    gf3d_wireframe.wireframe_max = wireframe_max;

    gf3d_wireframe.bindingDescription.binding = 0;
    gf3d_wireframe.bindingDescription.stride = sizeof(Vertex);
    gf3d_wireframe.bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    gf3d_wireframe.attributeDescriptions[0].binding = 0;
    gf3d_wireframe.attributeDescriptions[0].location = 0;
    gf3d_wireframe.attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    gf3d_wireframe.attributeDescriptions[0].offset = offsetof(Vertex, vertex);

    gf3d_wireframe.attributeDescriptions[1].binding = 0;
    gf3d_wireframe.attributeDescriptions[1].location = 1;
    gf3d_wireframe.attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    gf3d_wireframe.attributeDescriptions[1].offset = offsetof(Vertex, normal);

    gf3d_wireframe.attributeDescriptions[2].binding = 0;
    gf3d_wireframe.attributeDescriptions[2].location = 2;
    gf3d_wireframe.attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    gf3d_wireframe.attributeDescriptions[2].offset = offsetof(Vertex, texel);

    gf3d_wireframe.wireframe_list = gfc_allocate_array(sizeof(Mesh), wireframe_max);
    slog("wireframe system initialized");
}

VkVertexInputAttributeDescription* gf3d_wireframe_get_attribute_descriptions(Uint32* count)
{
    if (count)*count = ATTRIBUTE_COUNT;
    return gf3d_wireframe.attributeDescriptions;
}

VkVertexInputBindingDescription* gf3d_wireframe_get_bind_description()
{
    return &gf3d_wireframe.bindingDescription;
}

Mesh* gf3d_wireframe_new()
{
    int i;
    for (i = 0; i < gf3d_wireframe.wireframe_max; i++)
    {
        if (gf3d_wireframe.wireframe_list[i]._inuse == 0)
        {
            gf3d_wireframe.wireframe_list[i]._inuse = 1;
            return &gf3d_wireframe.wireframe_list[i];
        }
    }
    return NULL;
}


void gf3d_wireframe_free(Wireframe* wireframe)
{
    if (!wireframe)return;
}

void gf3d_wireframe_free_all()
{
    int i;
    for (i = 0; i < gf3d_wireframe.wireframe_max; i++)
    {
        gf3d_wireframe_delete(&gf3d_wireframe.wireframe_list[i]);
    }
}

void gf3d_wireframe_close()
{
    slog("cleaning up wireframe data");
    if (gf3d_wireframe.wireframe_list)
    {
        gf3d_wireframe_free_all();
        // TODO: iterate through mesh data and free all data
        free(gf3d_wireframe.wireframe_list);
        gf3d_wireframe.wireframe_list = NULL;
    }
    slog("wireframe system closed");
}

void gf3d_wireframe_delete(Wireframe* wireframe)
{
    if ((!wireframe) || (!wireframe->_inuse))return;
    if (wireframe->buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(gf3d_vgraphics_get_default_logical_device(), wireframe->buffer, NULL);
    }
    if (wireframe->bufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(gf3d_vgraphics_get_default_logical_device(), wireframe->bufferMemory, NULL);
    }
    memset(wireframe, 0, sizeof(Wireframe));
}

void gf3d_wireframe_scene_add(Wireframe* wireframe)
{
    if (!wireframe)return;
}

void gf3d_wireframe_render(Wireframe* wireframe, VkCommandBuffer commandBuffer, VkDescriptorSet* descriptorSet)
{
    VkDeviceSize offsets[] = { 0 };
    Pipeline* pipe;
    if (!wireframe)
    {
        slog("cannot render a NULL wireframe");
        return;
    }
    pipe = gf3d_vgraphics_get_wireframe_pipeline();
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &wireframe->buffer, offsets);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipe->pipelineLayout, 0, 1, descriptorSet, 0, NULL);

    //vkCmdDraw(commandBuffer, wireframe->vertexCount * 3, 1, 0, 0, 0);
}


void gf3d_wireframe_create_vertex_buffer_from_vertices(Wireframe* wireframe, Vertex* vertices, Uint32 vcount)
{
    void* data = NULL;
    VkDevice device = gf3d_vgraphics_get_default_logical_device();
    size_t bufferSize;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    bufferSize = sizeof(Vertex) * vcount;

    gf3d_vgraphics_create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices, (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    gf3d_vgraphics_create_buffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &wireframe->buffer, &wireframe->bufferMemory);

    gf3d_vgraphics_copy_buffer(stagingBuffer, wireframe->buffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, NULL);
    vkFreeMemory(device, stagingBufferMemory, NULL);

    wireframe->vertexCount = vcount;
    wireframe->bufferMemory = wireframe->bufferMemory;


    slog("created a wireframe with %i vertices", vcount);
}

Wireframe* gf3d_wireframe_setup(Vector3D position, Vector3D dimensions)
{
    Wireframe* wireframe;
    ObjData* obj;

    obj = (ObjData*)gfc_allocate_array(sizeof(ObjData), 1);

    if (!obj)
    {
        return NULL;
    }

    wireframe = gf3d_mesh_new();
    if (!wireframe)
    {
        return NULL;
    }


    gf3d_wireframe_create_vertex_buffer_from_vertices(wireframe, obj->faceVertices, obj->face_vert_count);
    gf3d_obj_free(obj);
    return NULL;
}
