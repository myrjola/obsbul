#include "../../common.h"

#include "../gamefw.h"

#include <physfs.h>

using namespace gamefw;

int main(int argc, char* argv[])
{
    PHYSFS_init(argv[0]);

    uint width = 800, height = 600;

    Game* game = new Game(width, height);
    
    int status= glewInit();
    if (GLEW_OK != status) {
        LOG(logERROR) << "Error:" << glewGetErrorString(status) << "\n";
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

    entity.m_position = glm::vec3(4.0f, 0.0f, -15.0f);
	entity2.m_position = glm::vec3(-4.0f, 0.0f, -10.0f);

    Renderer* renderer = new Renderer(width, height);

    sf::Clock clock;

    sf::Window* main_window = game->getMainWindow();
    main_window->ShowMouseCursor(false);

    while (true) {
        float time_since_draw = clock.GetElapsedTime();
        if (time_since_draw >= 1.0f/60) {
            clock.Reset();
            if (!game->update()) { // If window closing.
                break;
            }
            renderer->addToRenderQueue(entity);
            renderer->addToRenderQueue(entity2);
            renderer->render();
            main_window->Display();
        }
    }
    delete renderer;
    delete game;
    delete fileservice;
    delete shaderfactory;

    return 0;
}
