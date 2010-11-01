#include "objfile.h"

#include <sstream>
#include <fstream>
#include <stdexcept>

#include <boost/algorithm/string.hpp>

namespace util {

const char* CorruptObjFileException::what() const throw()
{
    return "Corrupt obj-file.";
}

ObjFile::ObjFile(const std::string& path)
        :
        m_path(path),
        m_num_positions(0),
        m_num_normals(0),
        m_num_texcoords(0),
        m_num_triangles(0),
        m_num_materials(0)
{
    ifstream file(path);

    assert(file.is_open());

    parseFile(file);

    file.close();
}

void ObjFile::parseFile(ifstream& file)
{
	streamsize LARGE_NUMBER = std::numeric_limits<std::streamsize>::max();
    uint line_number = 1;
    uint current_mtl = 0; // Default material.
    GLfloat v1, v2, v3;
    while (!file.eof()) {
        string definition = extractNextWord(file);
        if (boost::istarts_with(definition, "v")) { // Vertex varyings.
            file >> v1 >> v2;
            if (definition == "vt") { // Texcoord
                addTexcoord(v1, v2);
                
            } else {
                file >> v3;
            }
            
            // Bail out if incorrect format.
            if (file.bad()) {
                LOG(logERROR) << "Incorrect format in " <<
                    m_path << ":" << line_number;
                throw CorruptObjFileException();
            }

            if (definition == "v") { // Position.
                addPosition(v1, v2, v3);

            } else if (definition == "vn") { // Normal
                addNormal(v1, v2, v3);

            }

        } else if (definition == "mtllib") {
            string mtllib_name = extractNextWord(file);
            loadMaterials(mtllib_name);

        } else if (definition == "usemtl") {
            string mtl_name = extractNextWord(file);
            try {
                current_mtl = m_mtl_table.at(mtl_name);
            } catch (std::out_of_range) {
                LOG(logERROR) << "Unknown material in " <<
                m_path << ":" << line_number;
                throw CorruptObjFileException();
            }

        } else if (definition == "f") { // Triangle.
            // position, normal, texcoord.
            GLuint p1, t1, n1;
            GLuint p2, t2, n2;
            GLuint p3, t3, n3;

            char slash;
            // Recognize only %d/%d/%d.
            file >> p1 >> slash >> t1 >> slash >> n1;
            file >> p2 >> slash >> t2 >> slash >> n2;
            file >> p3 >> slash >> t3 >> slash >> n3;

            addTriangle(p1 - 1, t1 - 1, n1 - 1, p2 - 1, t2 - 1, n2 - 1,
                        p3 - 1, t3 - 1, n3 - 1, current_mtl);
        }

        // Next line.
        line_number++;
        file.ignore(LARGE_NUMBER, '\n');
    }
}

void ObjFile::addPosition(GLfloat x, GLfloat y, GLfloat z)
{
    m_positions.push_back(x);
    m_positions.push_back(y);
    m_positions.push_back(z);
    m_num_positions++;
}

void ObjFile::addNormal(GLfloat x, GLfloat y, GLfloat z)
{
    m_normals.push_back(x);
    m_normals.push_back(y);
    m_normals.push_back(z);
    m_num_normals++;
}

void ObjFile::addTexcoord(GLfloat u, GLfloat v)
{
    m_texcoords.push_back(u);
    m_texcoords.push_back(v);
    m_num_texcoords++;
}

void ObjFile::addTriangle(GLuint p1, GLuint t1, GLuint n1,
                          GLuint p2, GLuint t2, GLuint n2,
                          GLuint p3, GLuint t3, GLuint n3,
                          GLuint mtl_index)
{
    t_obj_triangle triangle = {
        {p1, p2, p3}, // pindices
        {n1, n2, n3}, // nindices
        {t1, t2, t3}, // tindices
        mtl_index
    };
    m_triangles.push_back(triangle);
    m_num_triangles++;
}

string ObjFile::extractNextWord(ifstream& stream)
{
    const int MAX_WORD_LENGTH = 260; // Will be sufficient;
    char word_char_array[MAX_WORD_LENGTH];
    word_char_array[0] = '\0'; // Initialize in case if empty line.
    stream.width(MAX_WORD_LENGTH); // To prevent buffer overflow.
    stream >> word_char_array;
    string word(word_char_array);
    return word;
}

void ObjFile::loadMaterials(string mtllib_name)
{
    streamsize LARGE_NUMBER = std::numeric_limits<std::streamsize>::max();
    string dirseparator = "/";
    string mtllib_path(m_path);
    int directory_pos = mtllib_path.find_last_of(dirseparator);
    mtllib_path.resize(directory_pos + 1); // Snip filename.obj.
    mtllib_path += mtllib_name;

    ifstream file(mtllib_path);
    assert(file.is_open());

    GLfloat r, g, b;

    string current_mtl_name = "default";
    t_obj_mtl current_material = getDefaultMtl();

    uint line_number = 1;
    while (!file.eof()) {
        string definition = extractNextWord(file);
        if (definition == "newmtl") {
            // Save the current material
            m_materials.push_back(current_material);
            m_mtl_table[current_mtl_name] = m_num_materials++;

            // Create the new material.
            current_material = getDefaultMtl();
            current_mtl_name = extractNextWord(file);

        } else if (boost::istarts_with(definition, "K")) { // Color.
            file >> r >> g >> b;
            GLfloat nothing[4];
            GLfloat* color = nothing;
            
            if (definition == "Kd") { // Diffuse.
                color = current_material.diffuse;
            } else if (definition == "Ks") { // Specular.
                color = current_material.specular;
            } 
            color[0] = r;
            color[1] = g;
            color[2] = b;

        } else if (definition == "Ns") { // Shininess.
            file >> current_material.shininess;

        } else if (definition == "d") { // Disolve factor ie. alpha value.
            GLfloat alpha;
            file >> alpha;
            current_material.diffuse[3] = alpha;
        }

        // Error checking.
        if (file.bad()) {
            LOG(logERROR) << "Invalid format in " <<
            m_path << ":" << line_number;
            throw CorruptObjFileException();
        }

        // Next line.
        line_number++;
        file.ignore(LARGE_NUMBER, '\n');
    }
    // Save the last material
    m_materials.push_back(current_material);
    m_mtl_table[current_mtl_name] = m_num_materials++;

    file.close();
}

t_obj_mtl ObjFile::getDefaultMtl()
{
    t_obj_mtl material = {
        {0.5f, 0.5f, 0.5f, 1.0f}, // diffuse
        {1.0f, 1.0f, 1.0f, 1.0f}, // specular
        128.0f,                   // shininess
        {0.0f, 0.0f, 0.0f}        // padding
    };
    return material;
}

const GLfloat* ObjFile::getNormals() const
{
    return &m_normals[0];
}

const t_obj_mtl* ObjFile::getMaterials() const
{
    return &m_materials[0];
}

const GLuint ObjFile::getNumMaterials() const
{
    return m_num_materials;
}

const GLuint ObjFile::getNumNormals() const
{
    return m_num_normals;
}

const GLuint ObjFile::getNumPositions() const
{
    return m_num_positions;
}

const GLuint ObjFile::getNumTexcoords() const
{
    return m_num_texcoords;
}

const GLuint ObjFile::getNumTriangles() const
{
    return m_num_triangles;
}

const GLfloat* ObjFile::getPositions() const
{
    return &m_positions[0];
}

const GLfloat* ObjFile::getTexCoords() const
{
    return &m_texcoords[0];
}

const t_obj_triangle* ObjFile::getTriangles() const
{
    return &m_triangles[0];
}

}

