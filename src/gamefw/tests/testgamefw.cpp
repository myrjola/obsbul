/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/


#include "../../common.h"

#include "../gamefw.h"
#include <GL/freeglut.h>
#include <physfs.h>


using namespace gamefw;

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    PHYSFS_init(argv[0]);

    Game* game = new Game();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    FileService* fileservice = new FileService();
    Locator::registerFileService(*fileservice);
    ShaderFactory* shaderfactory = new ShaderFactory();
    Locator::registerShaderFactory(*shaderfactory);

    Entity entity = Locator::getFileService().createEntity("sphere");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shared_ptr<RenderJob> renderjob = entity.getRenderJob();

    checkOGLError();
    
    glUseProgram(renderjob->m_shaderprogram);
    glBindVertexArray(renderjob->m_buffer_objects.vertex_buffer);

    glEnableVertexAttribArray(RenderJob::POSITION);
    glEnableVertexAttribArray(RenderJob::NORMAL);
    glEnableVertexAttribArray(RenderJob::TEXCOORD);
    glEnableVertexAttribArray(RenderJob::MATERIAL_IDX);
    glDrawElements(GL_TRIANGLES, renderjob->m_vertex_count, GL_UNSIGNED_SHORT, 0);
    glDisableVertexAttribArray(RenderJob::POSITION);
    glDisableVertexAttribArray(RenderJob::NORMAL);
    glDisableVertexAttribArray(RenderJob::TEXCOORD);
    glDisableVertexAttribArray(RenderJob::MATERIAL_IDX);

    sf::Window* main_window = game->getMainWindow();

    while (true) {
        game->update();
        main_window->Display();
    }
        
    delete fileservice;
    delete shaderfactory;
    
//     return UnitTest::RunAllTests();
    return 0;
}
