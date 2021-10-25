#ifndef __WIREFRAME_H__
#define __WIREFRAME_H__

#include <vulkan/vulkan.h>
#include "gfc_vector.h"
#include "gfc_text.h"
#include "gf3d_mesh.h"


typedef struct
{
    Uint8           _inuse;
    Uint32          vertexCount;
    VkBuffer        buffer;
    VkDeviceMemory  bufferMemory;
}Wireframe;

/**
 * @brief initializes the mesh system / configures internal data about mesh based rendering
 * @param mesh_max the maximum allowed simultaneous meshes supported at once.  Must be > 0
 */
void gf3d_wireframe_init(Uint32 wireframe_max);

/**
 * @brief get the input attribute descriptions for mesh based rendering
 * @param count (optional, output) the number of attributes
 * @return a pointer to a vertex input attribute description array
 */
VkVertexInputAttributeDescription* gf3d_wireframe_get_attribute_descriptions(Uint32* count);

/**
 * @brief get the binding description for mesh based rendering
 * @return vertex input binding descriptions compatible with mesh data
 */
VkVertexInputBindingDescription* gf3d_wireframe_get_bind_description();

/**
 * @brief free a mesh that has been loaded from memory
 */
void gf3d_wireframe_free(Wireframe* wireframe);

/**
 * @brief adds a mesh to the render pass
 * @note: must be called within the render pass
 * @param mesh the mesh to render
 * @param com the command pool to use to handle the request we are rendering with
 */
void gf3d_wireframe_render(Wireframe* wireframe, VkCommandBuffer commandBuffer, VkDescriptorSet* descriptorSet);

/**
 * @brief create a mesh's internal buffers based on vertices
 * @param mesh the mesh handle to populate
 * @param vertices an array of vertices to make the mesh with
 * @param vcount how many vertices are in the array
 */
void gf3d_wireframe_create_vertex_buffer_from_vertices(Wireframe* wireframe, Vertex* vertices, Uint32 vcount);



Wireframe* gf3d_wireframe_setup(Vector3D position, Vector3D dimensions);


#endif
