#ifndef OBJFILE_H
#define OBJFILE_H

#include <GL/glew.h>

#include "../common.h"

#include <map>

namespace util {

/**
 * @brief Exception thrown when the obj-file is corrupt.
 */
class CorruptObjFileException: public exception
{
public:
    /**
     * @return Description of exception.
     */
    virtual const char* what() const throw();
};

/**
 * @brief Mesh material representation.
 *
 * Doesn't include all features, such as textures.
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
} t_obj_mtl;

/**
 * @brief Triangle representation.
 *
 * Include vertex position, normal, texcoord and material indices.
 */
typedef struct {
    /// Vector position indices.
    GLuint pindices[3];
    /// Vector normal indices.
    GLuint nindices[3];
    /// Vector texcoord indices.
    GLuint tindices[3];
    /// Vector material index.
    GLuint material;
} t_obj_triangle;

/**
 * @brief OBJ-file handler.
 *
 * Simple class to parse an obj-file and then use the data for example into
 * OpenGL buffers.
 *
 * Face definitions must be of the following format:
 * \code
 * f %d/%d/%d %d/%d/%d %d/%d/%d
 * \endcode
 *
 * Usage:
 *  \code
 *  ObjFile* file = new ObjFile("file.obj"); // Automatically loads the file.
 *  // Use the public variables ie. file.getPositions()[10].
 *  delete file; // Remember to deallocate.
 *  \endcode
 */
class ObjFile {
    
public:
    /**
     * Load an obj-file.
     *
     * @param path Path to the file, including extensions.
     */
    ObjFile(const string& path );

    /// Getter for the amount of vertex positions in the mesh.
    const GLuint getNumPositions() const;
    /// Getter for the amount of vertex normals in the mesh.
    const GLuint getNumNormals() const;
    /// Getter for the amount of vertex texcoords in the mesh.
    const GLuint getNumTexcoords() const;
    /// Getter for the amount of triangles in the mesh.
    const GLuint getNumTriangles() const;
    /// Getter for the amount of materials in the mesh.
    const GLuint getNumMaterials() const;

    /**
     * @brief Get the vertex positions.
     *
     * @return An array of length getNumPositions().
     *         Remember that the positions are vec3:s.
     **/
    const GLfloat* getPositions() const;

    /**
     * @brief Get the vertex normals.
     *
     * @return An array of length getNumNormals().
     *         Remember that the normals are vec3:s.
     **/
    const GLfloat* getNormals() const;
    

    /**
     * @brief Get the vertex texcoords.
     *
     * @return An array of length getNumTexcoords().
     *         Remember that the texcoords are vec2:s.
     **/
    const GLfloat* getTexCoords() const;

    /**
     * @brief Get the triangles.
     *
     * @return An array of length getNumTriangles().
     **/
    const t_obj_triangle* getTriangles() const;

    /**
     * @brief Get the materials.
     *
     * @return An array of length getNumMaterials().
     **/
    const t_obj_mtl* getMaterials() const;

private:
    void parseFile(ifstream& file);
    void loadMaterials(string mtllib_name);
    
    void addTriangle(GLuint p1, GLuint n1, GLuint t1,
                     GLuint p2, GLuint n2, GLuint t2,
                     GLuint p3, GLuint n3, GLuint t3,
                     GLuint mtl_index);

    void addPosition(GLfloat x, GLfloat y, GLfloat z);
    void addNormal(GLfloat x, GLfloat y, GLfloat z);
    void addTexcoord(GLfloat u, GLfloat v);

    string extractNextWord(ifstream& stream);
    
    t_obj_mtl getDefaultMtl();

    GLuint m_num_positions;
    // std::vector of positions as vec3:s
    vector<GLfloat> m_positions;

    GLuint m_num_normals;
    // std::vector of normals as vec3:s
    vector<GLfloat> m_normals;

    GLuint m_num_texcoords;
    // std::vector of texcoords as vec2:s
    vector<GLfloat> m_texcoords;

    GLuint m_num_triangles;
    vector<t_obj_triangle> m_triangles;

    GLuint m_num_materials;
    vector<t_obj_mtl> m_materials;
    
    map<string, uint> m_mtl_table;
    string m_path;
};


}

#endif // OBJFILE_H