/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/

#include "entityfactory.h"

#include <boost/tokenizer.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

#define MATERIAL_BY_FACE
#include <glm.h>

#include "locator.h"


using namespace gamefw;

const char* EntityCreationError::what() const throw()
{
    return "Error when creating Entity.";
}

Entity& EntityFactory::createEntity(string path)
{
    Entity* entity = new Entity();
    shared_ptr<RenderJob> renderjob(new RenderJob());

    
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

    // Load model.
    TiXmlElement* model_element =
        dochandle.FirstChild("gfx").FirstChild("model").ToElement();
    if (!model_element) {
        DLOG(INFO) << "No model element in entity file " << path;
        throw EntityCreationError();
    }
    string modelpath = "assets/models/" + string(model_element->GetText()) + ".obj";
    string absolute_path(Locator::getFileService().getRealPath(modelpath));
    loadModel(absolute_path, renderjob);

    // Load shaders.
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
    boost::char_separator<char> comma(",");
    tokenizer tokens(shader_defines, comma);
    foreach(string define, tokens) {
        defines.insert(define);
    }

    ShaderFactory& shaderfactory = Locator::getShaderFactory();
    ShaderProgram& shaderprogram = shaderfactory.makeShader(defines);
    renderjob->m_shaderprogram = shaderprogram.getProgramID();
    
    DLOG(INFO) << "Entity "
                  << (name_element ? name_element->GetText() : "*UnNamed*")
                  << " created from " << path;
    return *entity;
}

void EntityFactory::loadModel(string& path, shared_ptr< RenderJob > renderjob)
{
    GLMmodel* model = glmReadOBJ(path.c_str());

    vector<t_vertex> vertex_buffer;
    vector<GLushort> element_buffer;
    //  tuple<vertex, normal, texcoord, material_idx>
    typedef boost::tuple<int, int, int, int> vec_identifier;
    map<vec_identifier, int> vec_indexes;

    int numtriangles = model->numtriangles;

    // Create vertex- and element buffers.
    for (int i = 0; i < numtriangles; i++) {
        GLMtriangle* triangle = model->triangles + i;
        int material_idx = triangle->material;
        for (int j = 0; j < 3; j++) {
            int pos, nor, tex;
            pos = triangle->vindices[j];
            nor = triangle->nindices[j];
            tex = triangle->tindices[j];
            vec_identifier id = boost::make_tuple(pos, nor, tex, material_idx);
            map<vec_identifier, int>::iterator result = vec_indexes.find(id);
            
            if (result == vec_indexes.end()) { // If vertex not created.
                t_vertex vertex;
                memcpy(vertex.position, model->vertices + pos, sizeof(GLfloat) * 3);
                memcpy(vertex.normal, model->normals + nor, sizeof(GLfloat) * 3);
                memcpy(vertex.texcoord, model->texcoords + tex, sizeof(GLfloat) * 2);
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

    glGenVertexArrays(1, &renderjob->m_buffer_objects.vao);
    glBindVertexArray(renderjob->m_buffer_objects.vao);
    glGenBuffers(1, &renderjob->m_buffer_objects.vertex_buffer);
    glGenBuffers(1, &renderjob->m_buffer_objects.element_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, renderjob->m_buffer_objects.vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer.size() * sizeof(t_vertex),
        &vertex_buffer[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(
        RenderJob::POSITION,
        3, GL_FLOAT, GL_FALSE, sizeof(t_vertex),
        (void*) offsetof(t_vertex, position)
    );
                          
    glVertexAttribPointer(
        RenderJob::NORMAL,
        3, GL_FLOAT, GL_FALSE, sizeof(t_vertex),
        (void*) offsetof(t_vertex, normal)
    );
                          
    glVertexAttribPointer(
        RenderJob::TEXCOORD,
        2, GL_FLOAT, GL_FALSE, sizeof(t_vertex),
        (void*) offsetof(t_vertex, texcoord)
    );
                          
    glVertexAttribPointer(
        RenderJob::MATERIAL_IDX,
        3, GL_UNSIGNED_INT, GL_FALSE, sizeof(t_vertex),
        (void*) offsetof(t_vertex, material_idx)
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderjob->m_buffer_objects.element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_buffer.size() * sizeof(GLushort),
                 &element_buffer[0], GL_STATIC_DRAW);
                          
    glmDelete(model);
}
