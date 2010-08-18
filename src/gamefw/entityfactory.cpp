
#include "entityfactory.h"

#include <sstream>

#include <boost/tokenizer.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

#define MATERIAL_BY_FACE // To get material indices.
#include <glm.h>

using namespace gamefw;

const char* EntityCreationError::what() const throw()
{
    return "Error when creating Entity.";
}

Entity& EntityFactory::createEntity(string path)
{
    Entity* entity = new Entity();
    shared_ptr<RenderJob> renderjob(new RenderJob());
    entity->setRenderJob(renderjob);
    
    m_current_entityfile = new TiXmlDocument(path);
    if (!m_current_entityfile->LoadFile()) { // If error when loading file.
        DLOG(ERROR) << "Error when loading entity file " << path <<
        "\nError at line " << m_current_entityfile->ErrorRow() <<
        "\nError description: " << m_current_entityfile->ErrorDesc();
        throw EntityCreationError();
    }

    TiXmlHandle dochandle = TiXmlHandle(m_current_entityfile).FirstChild("entity");

    // Set name and description.
    
    TiXmlElement* name_element = dochandle.FirstChild("name").ToElement();
    if (name_element) 
        entity->setName(name_element->GetText());
    else
        DLOG(WARNING) << "No name element in entity file " << path;
    TiXmlElement* desc_element = dochandle.FirstChild("desc").ToElement();
    if (desc_element) {
        entity->setDesc(desc_element->GetText());
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

        renderjob->m_textures = new GLuint[created_textures.size()];
        for (int i = 0; i < created_textures.size(); i++) {
            renderjob->m_textures[i] = created_textures[i];
        }
    }
    
    // Load shaders.
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
        
        // Load model and check number of materials.
        TiXmlElement* model_element =
            dochandle.FirstChild("gfx").FirstChild("model").ToElement();
        if (!model_element) {
            DLOG(ERROR) << "No model element in entity file " << path;
            throw EntityCreationError();
        }
        string modelpath = "assets/models/" + string(model_element->GetText()) + ".obj";
        string absolute_path(Locator::getFileService().getRealPath(modelpath));
        GLMmodel* model = glmReadOBJ(absolute_path.c_str());
        GLuint num_materials = model->nummaterials;
        string materials_define("MATERIALS");
        if (defines.find(materials_define) != defines.end()) { // If materials define found.
            defines.erase(materials_define);
            stringstream materials_define_stream;
            materials_define_stream << materials_define << " " << num_materials;
            defines.insert(materials_define_stream.str());
        }

        ShaderFactory& shaderfactory = Locator::getShaderFactory();
        ShaderProgram& shaderprogram = shaderfactory.makeShader(defines);
        renderjob->m_shaderprogram = shaderprogram.getProgramID();
    }

    // Load model after shader creation because uniform blocks
    // needs a working shader program.
    loadModel(model, renderjob);

    DLOG(INFO) << "Entity "
                  << (name_element ? name_element->GetText() : "*UnNamed*")
                  << " created from " << path;
    return *entity;
}

void EntityFactory::loadModel(GLMmodel* model, shared_ptr< RenderJob > renderjob)
{

    vector<t_vertex> vertex_buffer;
    vector<GLushort> element_buffer;
    //  tuple<vertex, normal, texcoord, material_idx>
    typedef boost::tuple<int, int, int, int> vec_identifier;
    map<vec_identifier, int> vec_indexes;

    int numtriangles = model->numtriangles;

    // Create vertex- and element buffers.
    for (int i = 0; i < numtriangles; i++) {
        GLMtriangle* triangle = model->triangles + i;
        GLuint material_idx = triangle->material;
        for (int j = 0; j < 3; j++) {
            int pos, nor, tex;
            pos = triangle->vindices[j];
            nor = triangle->nindices[j];
            tex = triangle->tindices[j];
            vec_identifier id = boost::make_tuple(pos, nor, tex, material_idx);
            map<vec_identifier, int>::iterator result = vec_indexes.find(id);
            
            if (result == vec_indexes.end()) { // If vertex not created.
                t_vertex vertex;
                memcpy(vertex.position, model->vertices + pos * 3, sizeof(GLfloat) * 3);
                vertex.position[3] = 1.0;
                memcpy(vertex.normal, model->normals + nor * 3, sizeof(GLfloat) * 3);
                vertex.normal[3] = 0.0;
                memcpy(vertex.texcoord, model->texcoords + tex * 2, sizeof(GLfloat) * 2);
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

    createMaterials(renderjob, model);


    checkOpenGLError();
    glmDelete(model);
}

void EntityFactory::genVertexBuffers(shared_ptr<RenderJob> renderjob,
        t_vertex* vertex_buffer, size_t vertex_buffer_length,
        GLushort* element_buffer, size_t element_buffer_length)
{
    glGenVertexArrays(1, &renderjob->m_buffer_objects.vao);
    glBindVertexArray(renderjob->m_buffer_objects.vao);
    {
        glGenBuffers(1, &renderjob->m_buffer_objects.vertex_buffer);
        glGenBuffers(1, &renderjob->m_buffer_objects.element_buffer);

        glBindBuffer(GL_ARRAY_BUFFER, renderjob->m_buffer_objects.vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, vertex_buffer_length * sizeof(t_vertex),
            vertex_buffer, GL_STATIC_DRAW);

        glVertexAttribPointer(
            RenderJob::POSITION,
            4, GL_FLOAT, GL_FALSE, sizeof(t_vertex),
            (void*) offsetof(t_vertex, position)
        );

        glVertexAttribPointer(
            RenderJob::NORMAL,
            4, GL_FLOAT, GL_FALSE, sizeof(t_vertex),
            (void*) offsetof(t_vertex, normal)
        );

        glVertexAttribPointer(
            RenderJob::TEXCOORD,
            2, GL_FLOAT, GL_FALSE, sizeof(t_vertex),
            (void*) offsetof(t_vertex, texcoord)
        );

        glVertexAttribIPointer(
            RenderJob::MATERIAL_IDX,
            1, GL_UNSIGNED_INT, sizeof(t_vertex),
            (void*) offsetof(t_vertex, material_idx)
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderjob->m_buffer_objects.element_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_buffer_length * sizeof(GLushort),
                    element_buffer, GL_STATIC_DRAW);
    }
    glBindVertexArray(0);
}

void EntityFactory::createMaterials(shared_ptr<RenderJob> renderjob, GLMmodel* model)
{
    int program_id = renderjob->m_shaderprogram;
    int num_materials = model->nummaterials;

    // Create materials and bind them to uniform block.
    t_material materials[num_materials];
    for (int i = 0; i < num_materials; i++) {
        model->materials[i].diffuse;
        memcpy(materials[i].diffuse, model->materials[i].diffuse,
               sizeof(GLfloat) * 4);
        memcpy(materials[i].specular, model->materials[i].specular,
               sizeof(GLfloat) * 4);
        materials[i].shininess = model->materials[i].shininess;
    }
    
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
    assert(block_size == sizeof(t_material) * num_materials);

    // Create Uniform Buffer Object and fill with material data.
    glGenBuffers(1, &renderjob->m_uniforms.materials);
    glBindBuffer(GL_UNIFORM_BUFFER, renderjob->m_uniforms.materials);
    glBufferData(GL_UNIFORM_BUFFER, block_size, materials, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Attach the UBO to RenderJob::MATERIAL index.
    glBindBufferBase(GL_UNIFORM_BUFFER, RenderJob::MATERIAL, renderjob->m_uniforms.materials);
    // Associate the block in the GLSL source to this index.
    glUniformBlockBinding(program_id, material_location, RenderJob::MATERIAL);

}

