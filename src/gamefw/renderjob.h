#ifndef RENDERJOB_H
#define RENDERJOB_H

#include "../common.h"
#include <boost/preprocessor.hpp>

#include "shaderprogram.h"

namespace gamefw {

#define T_VERTEX (POSITION)(NORMAL)(TEXCOORD)(MATERIAL_IDX)
#define T_VERTEX_EXTRA (TANGENT)(BITANGENT)
#define UNIFORM_BLOCKS (MATERIAL)(GLOBAL)(TRANSFORM)
#define OUT_GBUFFERS (OUTG_DIFFUSE)(OUTG_SPECULAR)(OUTG_NORMAL)(OUTG_POSITION)
#define OUT_PBUFFERS (OUTP_DIFFUSE)(OUTP_SPECULAR)(OUTP_EDGES)(OUTP_BLOOM)
#define OUT_POSTPROCBUFFERS (OUTPP_DIFFUSE)(OUTPP_SPECULAR)

#define TO_STR(unused,data,elem) BOOST_PP_STRINGIZE(elem) ,

namespace renderjob_enums {
    // Automatically generate enums and corresponding strings.
    enum vertex_attribs { BOOST_PP_SEQ_ENUM(T_VERTEX) };
    enum vertex_extra_attribs { BOOST_PP_SEQ_ENUM(T_VERTEX_EXTRA) };
    enum uniform_blocks { BOOST_PP_SEQ_ENUM(UNIFORM_BLOCKS) };
    enum out_gbuffers { BOOST_PP_SEQ_ENUM(OUT_GBUFFERS) };
    enum out_pbuffers { BOOST_PP_SEQ_ENUM(OUT_PBUFFERS) };
    enum out_ppbuffers { BOOST_PP_SEQ_ENUM(OUT_POSTPROCBUFFERS) };
    static const char* vertex_strings[] = { BOOST_PP_SEQ_FOR_EACH(TO_STR,~,T_VERTEX) };
    static const char* vertex_extra_strings[] = {BOOST_PP_SEQ_FOR_EACH(TO_STR,~,T_VERTEX_EXTRA)};
    static const char* uniform_block_strings[] = {BOOST_PP_SEQ_FOR_EACH(TO_STR,~,UNIFORM_BLOCKS)};
    static const char* out_gbuffers_strings[] = {BOOST_PP_SEQ_FOR_EACH(TO_STR,~,OUT_GBUFFERS)};
    static const char* out_pbuffers_strings[] = {BOOST_PP_SEQ_FOR_EACH(TO_STR,~,OUT_PBUFFERS)};
    static const char* out_ppbuffers_strings[] = {BOOST_PP_SEQ_FOR_EACH(TO_STR,~,OUT_POSTPROCBUFFERS)};
}

#undef T_VERTEX 
#undef T_VERTEX_EXTRA
#undef UNIFORM_BLOCKS
#undef OUT_BUFFERS
 
/**
 * @brief Mesh representation sent to rendering.
 */
class RenderJob
{
public:
    RenderJob();
    ~RenderJob();

    void setShaderProgram(shared_ptr<ShaderProgram> m_shaderprogram);
    GLuint getShaderProgramID();

    /// OpenGL buffer objects.
    struct {
        GLuint vao, vertex_buffer,
               vertex_extra_buffer, element_buffer;
    } m_buffer_objects;

    /// Array of textures.
    GLuint* m_textures;

    /// Number of textures. The destructor uses this value to deallocate m_textures.
    GLuint m_num_textures;

    /// The shader program uniforms.
    struct {
        GLuint materials;
    } m_uniforms;


    /// Number of vertices in the model.
    int m_vertex_count;
    
private:
    shared_ptr<ShaderProgram> m_shaderprogram;
};

}

#endif // RENDERJOB_H
