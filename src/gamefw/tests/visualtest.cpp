
#include <GL/glew.h>
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
    
    int status= glewInit();
    if (GLEW_OK != status) {
        DLOG(ERROR) << "Error:" << glewGetErrorString(status) << "\n";
    }

    // The reason why I imported glew: arb_debug_output.
    // Doesn't yet work on nvidias' drivers.
    //     glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL,
    //                              GL_TRUE);
    //     glDebugMessageCallbackARB(&checkOpenGLError, NULL);
    

    FileService* fileservice = new FileService();
    Locator::registerFileService(*fileservice);
    ShaderFactory* shaderfactory = new ShaderFactory();
    Locator::registerShaderFactory(*shaderfactory);

    Entity entity = Locator::getFileService().createEntity("flatsmooth");
    Entity entity2 = Locator::getFileService().createEntity("sphere");

    Renderer* renderer = new Renderer();

    sf::Window* main_window = game->getMainWindow();

    while (true) {
        game->update();
        renderer->addToRenderQueue(entity);
//         render_context->addToRenderQueue(entity2);
        renderer->render();
        main_window->Display();
    }
        
    delete fileservice;
    delete shaderfactory;
    
    return 0;
}
