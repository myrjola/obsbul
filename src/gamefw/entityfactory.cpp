
#include "entityfactory.h"

#include <sstream>

#include <boost/tokenizer.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>


/**
 * @brief Vertex representation.
 *
 * GPU:s like 128-bit (4 floats) aligned buffers.
 */
typedef struct _vertex {
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
typedef struct _vertex_extra {
    /// Vertex surface tangent.
    GLfloat tangent[4];
    /// Vertex surface bitangent.
    GLfloat bitangent[4];
} t_vertex_extra;


/**
 * @brief Vertex representation.
 *
 * GPU:s like 128-bit (4 floats) aligned buffers.
 */
typedef struct _vertex {
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
typedef struct _vertex_extra {
    /// Vertex surface tangent.
    GLfloat tangent[4];
    /// Vertex surface bitangent.
    GLfloat bitangent[4];
} t_vertex_extra;

/**
 * @brief Mesh material properties.
 */
typedef struct _t_material {
    /// Diffuse color.
    GLfloat diffuse[4];
    /// Specular color.
    GLfloat specular[4];
    /// Shininess coefficient.
    GLfloat shininess;
    /// Padding needed for std140 layout to align properly.
    GLfloat padding[3]; // Needed for std140 layout.
} t_material;
 

using namespace gamefw;

const char* EntityCreationError::what() const throw()
{
    return "Error when creating Entity.";
}

Entity EntityFactory::createEntity(const string& path)
{
    Entity entity;
    shared_ptr<RenderJob> renderjob(new RenderJob());
    entity.setRenderJob(renderjob);

    TiXmlDocument current_entityfile(path);
    if (!current_entityfile.LoadFile()) { // If error when loading file.
        DLOG(ERROR) << "Error when loading entity file " << path <<
        "\nError at line " << current_entityfile.ErrorRow() <<
        "\nError description: " << current_entityfile.ErrorDesc();
        throw EntityCreationError();
    }

    TiXmlHandle dochandle = TiXmlHandle(&current_entityfile).FirstChild("entity");

    // Set name and description.

    TiXmlElement* name_element = dochandle.FirstChild("name").ToElement();
    if (name_element)
        entity.setName(name_element->GetText());
    else
        DLOG(WARNING) << "No name element in entity file " << path;
    TiXmlElement* desc_element = dochandle.FirstChild("desc").ToElement();
    if (desc_element) {
        entity.setDesc(desc_element->GetText());
    }
    else {
        DLOG(WARNING) << "No desc element in entity file " << path;
    }

    /*
     * Load gfx.
     */

    // Textures are loaded to an array in the order they are declared. My shader
    // code assumes that the albedo texture is index 0 and the normal map in
    // index 1.
    TiXmlHandle texhandle = dochandle.FirstChild("gfx").FirstChild("textures");
    if (texhandle.ToNode()) { // If texture tag exists.
        vector<GLuint> created_textures;
        TiXmlElement* texture_element;
        int i = 0;
        while (texture_element = texhandle.ChildElement(i++).ToElement()) {
            string texname(texture_element->GetText());
            created_textures.push_back(Locator::getFileService().makeTexture(texname));
        }

        int num_textures = created_textures.size();

        renderjob->m_textures = new GLuint[num_textures];
        renderjob->m_num_textures = num_textures;;
        for (int i = 0; i < num_textures; i++) {
            renderjob->m_textures[i] = created_textures[i];
        }
    }


    // Load model and check number of materials.
    TiXmlElement* model_element =
        dochandle.FirstChild("gfx").FirstChild("model").ToElement();
    if (!model_element) {
        DLOG(ERROR) << "No model element in entity file " << path;
        throw EntityCreationError();
    }
    string modelpath = "assets/models/" + string(model_element->GetText()) + ".obj";
    string absolute_path(Locator::getFileService().getRealPath(modelpath));
    ObjFile model(absolute_path);

    // Load shaders.
    bool materials_defined = false; // Needed to determine whether uniform blocks are created.
    {
        TiXmlElement* shader_defines_element =
            dochandle.FirstChild("gfx").FirstChild("shader_defines").ToElement();
        if (!shader_defines_element) {
            DLOG(ERROR) << "No shader_defines element in entity file " << path;
            throw EntityCreationError();
        }
        set<string> defines;
        string shader_defines(shader_defines_element->GetText());
        // Tokenize shader_defines;
        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
        boost::char_separator<char> comma(", ");
        tokenizer tokens(shader_defines, comma);
        foreach(string define, tokens) {
            defines.insert(define);
        }
        GLuint num_materials = model.getNumMaterials();
        string materials_define("MATERIALS");
        if (defines.find(materials_define) != defines.end()) { // If materials define found.
            materials_defined = true;
            defines.erase(materials_define);
            stringstream materials_define_stream;
            materials_define_stream << materials_define << " " << num_materials;
            defines.insert(materials_define_stream.str());
        }

        // Insert vertex attrib indices.
        int i = 0;
        foreach(const char* enum_name, renderjob_enums::vertex_strings) {
            defines.insert(makeDefineFromEnum(enum_name, i++));
        }
        i = 0;
        foreach(const char* enum_name, renderjob_enums::vertex_extra_strings) {
            defines.insert(makeDefineFromEnum(enum_name, i++));
        }
        i = 0;
        foreach(const char* enum_name, renderjob_enums::uniform_block_strings) {
            defines.insert(makeDefineFromEnum(enum_name, i++));
        }
        i = 0;
        foreach(const char* enum_name, renderjob_enums::out_gbuffers_strings) {
            defines.insert(makeDefineFromEnum(enum_name, i++));
        }
        i = 0;
        foreach(const char* enum_name, renderjob_enums::out_pbuffers_strings) {
            defines.insert(makeDefineFromEnum(enum_name, i++));
        }
        i = 0;
        foreach(const char* enum_name, renderjob_enums::out_ppbuffers_strings) {
            defines.insert(makeDefineFromEnum(enum_name, i++));
        }

        ShaderFactory& shaderfactory = Locator::getShaderFactory();
        shared_ptr<ShaderProgram> shaderprogram = shaderfactory.makeShader(defines);
        renderjob->setShaderProgram(shaderprogram);
    }

    // Load model after shader creation because uniform blocks
    // needs a working shader program.
    loadModel(*model, renderjob);

    if (materials_defined) {
        createMaterials(renderjob, *model);
        checkOpenGLError();
    }

    DLOG(INFO) << "Entity "
    << (name_element ? name_element->GetText() : "*UnNamed*")
    << " created from " << path;
    checkOpenGLError();
    return entity;
}

void EntityFactory::loadModel(const ObjFile& model, shared_ptr< RenderJob > renderjob)
{
    vector<t_vertex> vertex_buffer;
    vector<GLushort> element_buffer;
    //  tuple<vertex, normal, texcoord, material_idx>
    typedef boost::tuple<int, int, int, int> vec_identifier;
    map<vec_identifier, int> vec_indexes;

    int numtriangles = model.getNumTriangles();

    // Create vertex- and element buffers.
    for (int i = 0; i < numtriangles; i++) {
        const t_obj_triangle* triangle = model.getTriangles() + i;
        GLuint material_idx = triangle->material;
        for (int j = 0; j < 3; j++) {
            int pos, nor, tex;
            pos = triangle->pindices[j];
            nor = triangle->nindices[j];
            tex = triangle->tindices[j];
            vec_identifier id = boost::make_tuple(pos, nor, tex, material_idx);
            map<vec_identifier, int>::iterator result = vec_indexes.find(id);

            if (result == vec_indexes.end()) { // If vertex not created.
                t_vertex vertex;
                memcpy(vertex.position, model.getPositions() + pos * 3,
                       sizeof(GLfloat) * 3);
                vertex.position[3] = 1.0;
                memcpy(vertex.normal, model.getNormals() + nor * 3,
                       sizeof(GLfloat) * 3);
                vertex.normal[3] = 0.0;
                memcpy(vertex.texcoord, model.getTexCoords() + tex * 2,
                       sizeof(GLfloat) * 2);
                vertex.material_idx = material_idx;
                int vert_idx = vertex_buffer.size();
                vertex_buffer.push_back(vertex);
                vec_indexes[id] = vert_idx;
                result = vec_indexes.find(id);
                // TODO: tangent and bitangent calculations.
            }
            GLushort vertex_index = (GLushort) result->second;
            element_buffer.push_back(vertex_index);
        }
    }
    renderjob->m_vertex_count = element_buffer.size();

    genVertexBuffers(renderjob, &vertex_buffer[0], vertex_buffer.size(),
                     &element_buffer[0], element_buffer.size());
    checkOpenGLError();
}

const string EntityFactory::makeDefineFromEnum(const char* enum_name, int index) const
{
    stringstream enum_define;
    enum_define << enum_name;
    enum_define << " ";
    enum_define << index;
    return enum_define.str();
}


void EntityFactory::genVertexBuffers(shared_ptr<RenderJob> renderjob,
                                     const t_vertex* vertex_buffer,
                                     size_t vertex_buffer_length,
                                     const GLushort* element_buffer,
                                     size_t element_buffer_length) const
{
    glGenVertexArrays(1, &renderjob->m_buffer_objects.vao);
    glBindVertexArray(renderjob->m_buffer_objects.vao);
    {
        glGenBuffers(1, &renderjob->m_buffer_objects.vertex_buffer);
        glGenBuffers(1, &renderjob->m_buffer_objects.element_buffer);
        
        glBindBuffer(GL_ARRAY_BUFFER, renderjob->m_buffer_objects.vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, vertex_buffer_length * sizeof(t_vertex),
                     vertex_buffer, GL_STATIC_DRAW);
        checkOpenGLError();


        glVertexAttribPointer(
            renderjob_enums::POSITION,
            4, GL_FLOAT, GL_FALSE, sizeof(t_vertex),
            (void*) offsetof(t_vertex, position)
        );

        glVertexAttribPointer(
            renderjob_enums::NORMAL,
            4, GL_FLOAT, GL_FALSE, sizeof(t_vertex),
            (void*) offsetof(t_vertex, normal)
        );

        glVertexAttribPointer(
            renderjob_enums::TEXCOORD,
            2, GL_FLOAT, GL_FALSE, sizeof(t_vertex),
            (void*) offsetof(t_vertex, texcoord)
        );

        glVertexAttribIPointer(
            renderjob_enums::MATERIAL_IDX,
            1, GL_UNSIGNED_INT, sizeof(t_vertex),
            (void*) offsetof(t_vertex, material_idx)
        );
        checkOpenGLError();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderjob->m_buffer_objects.element_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_buffer_length * sizeof(GLushort),
                     element_buffer, GL_STATIC_DRAW);
    }
    glBindVertexArray(0);
}

void EntityFactory::createMaterials(shared_ptr<RenderJob> renderjob,
                                    const ObjFile& model) const
{
    int program_id = renderjob->getShaderProgramID();
    int num_materials = model.getNumMaterials();

    GLuint material_location = glGetUniformBlockIndex(program_id,
                               "materials");
    assert(material_location != GL_INVALID_INDEX);

    GLint block_size = 0;
    glGetActiveUniformBlockiv(
        program_id,
        material_location,
        GL_UNIFORM_BLOCK_DATA_SIZE,
        &block_size);

    // Tests if the the uniform block is similarly aligned in the buffer and the shader source.
    assert(block_size == sizeof(t_obj_mtl) * num_materials);

    // Create Uniform Buffer Object and fill with material data.
    glGenBuffers(1, &renderjob->m_uniforms.materials);
    glBindBuffer(GL_UNIFORM_BUFFER, renderjob->m_uniforms.materials);
    glBufferData(GL_UNIFORM_BUFFER, block_size, model.getMaterials(), GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Attach the UBO to RenderJob::MATERIAL index.
    glBindBufferBase(GL_UNIFORM_BUFFER, renderjob_enums::MATERIAL,
                     renderjob->m_uniforms.materials);
    // Associate the block in the GLSL source to this index.
    glUniformBlockBinding(program_id, material_location, renderjob_enums::MATERIAL);

}

