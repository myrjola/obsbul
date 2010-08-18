#ifndef RENDERJOB_H
#define RENDERJOB_H

#include <boost/preprocessor.hpp>

#include "gamefw.h"
   

namespace gamefw {

#define T_VERTEX (POSITION)(NORMAL)(TEXCOORD)(MATERIAL_IDX)
#define T_VERTEX_EXTRA (TANGENT)(BITANGENT)
#define UNIFORM_BLOCKS (MATERIAL)(GLOBAL)(TRANSFORM)

#define TO_STR(unused,data,elem) BOOST_PP_STRINGIZE(elem) ,

namespace renderjob_enums {
    // Automatically generate enums and corresponding strings.
    enum vertex_attribs { BOOST_PP_SEQ_ENUM(T_VERTEX) };
    enum vertex_extra_attribs { BOOST_PP_SEQ_ENUM(T_VERTEX_EXTRA) };
    enum uniform_blocks { BOOST_PP_SEQ_ENUM(UNIFORM_BLOCKS) };
    static const char* vertex_strings[] = { BOOST_PP_SEQ_FOR_EACH(TO_STR,~,T_VERTEX) };
    static const char* vertex_extra_strings[] = {BOOST_PP_SEQ_FOR_EACH(TO_STR,~,T_VERTEX_EXTRA)};
    static const char* uniform_block_strings[] = {BOOST_PP_SEQ_FOR_EACH(TO_STR,~,UNIFORM_BLOCKS)};
}

#undef T_VERTEX 
#undef T_VERTEX_EXTRA
#undef UNIFORM_BLOCKS
 
/**
 * @brief Mesh representation sent to rendering.
 */
class RenderJob
{
public:
    RenderJob();
    ~RenderJob();

   
    struct {
        GLuint vao, vertex_buffer,
               vertex_extra_buffer, element_buffer;
    } m_buffer_objects;

    GLuint* m_textures;
    GLuint num_textures;

    struct {
        GLuint materials;
        GLuint transforms;
    } m_uniforms;

    GLuint m_shaderprogram;
    
    int m_vertex_count;
};

}

#endif // RENDERJOB_H
