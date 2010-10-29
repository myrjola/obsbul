
#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "../common.h"

#include "gamefw.h"

#define TIXML_USE_STL
#include <tinyxml.h>

struct _GLMmodel;
typedef _GLMmodel GLMmodel;

/**
 * @brief Vertex representation.
 *
 * GPU:s like 128-bit (4 floats) aligned buffers.
 */
typedef struct {
    /// Vertex position.
    GLfloat position[4];
    /// Vertex surface normal.
    GLfloat normal[4];
    /// Vertex uv texture coordinate.
    GLfloat texcoord[2];
    /// Vertex material index.
    GLuint material_idx;
} t_vertex;

/**
 * @brief Extra buffers for the vertex representation.
 */
typedef struct {
    /// Vertex surface tangent.
    GLfloat tangent[4];
    /// Vertex surface bitangent.
    GLfloat bitangent[4];
} t_vertex_extra;

/**
 * @brief Mesh material properties.
 */
typedef struct {
    /// Diffuse color.
    GLfloat diffuse[4];
    /// Specular color.
    GLfloat specular[4];
    /// Shininess coefficient.
    GLfloat shininess;
    /// Padding needed for std140 layout to align properly.
    GLfloat padding[3]; // Needed for std140 layout.
} t_material;
 
namespace gamefw {

/**
 * @brief Thrown when EntityFactory fails in the creation of an Entity.
 */
class EntityCreationError: public exception
{
    /**
     * @return Description of exception.
     */
    virtual const char* what() const throw();
};

/**
 * @brief Takes care of Entity creation.
 *
 * Is used inside FileService to serve entities to the GameWorld. Parses a xml-
 * file and initializes an Entity according to it.
 */
class EntityFactory
{
public:
    /**
     * Constructs an Entity from it's file. The format is simple XML.
     *
     * @throw EntityCreationError When the creation fails.
     *
     * @param path The absolute path to the Entity's file.
     * @return Reference to the constructed Entity.
     */
    Entity createEntity(string path);

private:
    void loadModel(GLMmodel* path, shared_ptr<RenderJob> renderjob);
    
    void genVertexBuffers(shared_ptr<RenderJob> renderjob,
            t_vertex* vertex_buffer, size_t vertex_buffer_length,
            GLushort* element_buffer, size_t element_buffer_length);

    void createMaterials(shared_ptr<RenderJob> renderjob, GLMmodel* model);

    string makeDefineFromEnum(const char* attrib_name, int index);
};

}

#endif // ENTITYFACTORY_H
