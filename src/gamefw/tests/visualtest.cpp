#include "../../common.h"

#include "../gamefw.h"

#include "../convenience/defaultfirstpersoncontroller.h"

#include <physfs.h>

using namespace gamefw;

int main(int argc, char* argv[])
{
    PHYSFS_init(argv[0]);

    uint width = 1024, height = 768;

    Game* game = new Game(width, height);
    
    int status= glewInit();
    if (GLEW_OK != status) {
        LOG(logERROR) << "Error:" << glewGetErrorString(status) << "\n";
    }

    FileService* fileservice = new FileService();
    Locator::registerFileService(*fileservice);
    ShaderFactory* shaderfactory = new ShaderFactory();
    Locator::registerShaderFactory(*shaderfactory);

    Entity entity = Locator::getFileService().createEntity("flatsmooth");
    Entity entity2 = Locator::getFileService().createEntity("lightball");
    Entity entity3 = Locator::getFileService().createEntity("sphere");

    entity.m_position = glm::vec3(5.0f, 0.0f, -15.0f);
	entity2.m_position = glm::vec3(5.0f, 0.0f, -5.0f);
	entity3.m_position = glm::vec3(0.0f, 0.0f, -5.0f);

    Renderer* renderer = new Renderer(width, height);

    sf::Clock clock;

    sf::Window* main_window = game->getMainWindow();

    shared_ptr<Entity> camera(new Entity);
    DefaultFirstPersonController controller(camera);
    
    renderer->changeCamera(camera);
    game->changeController(&controller);

    renderer->m_pointlight = shared_ptr<Entity>(&entity2);

	float timer = 0.0f;
    while (true) {
        float time_since_draw = clock.GetElapsedTime();
        if (time_since_draw >= 1.0f/60) {
            camera->m_position.x += camera->m_velocity_local.x *
                            glm::cos(glm::radians(camera->m_orientation.yaw)) +
                            -camera->m_velocity_local.z *
                            glm::sin(glm::radians(camera->m_orientation.yaw));
            camera->m_position.z += camera->m_velocity_local.x *
                            glm::sin(glm::radians(camera->m_orientation.yaw)) +
                            camera->m_velocity_local.z *
                            glm::cos(glm::radians(camera->m_orientation.yaw));
            camera->m_position.y += camera->m_velocity_local.y;
            clock.Reset();
            if (!game->update()) { // If window closing.
                break;
            }
			entity2.m_position.x = 14.0f * glm::sin(timer);
			entity2.m_position.y = 14.0f * glm::cos(timer);
			timer += 0.01f;
			entity.m_orientation.pitch += 0.01f;
			entity2.m_orientation.roll += 0.03f;
			entity3.m_orientation.yaw += 0.02f;
			entity3.m_orientation.pitch += 0.005f;
            renderer->addToRenderQueue(entity);
            renderer->addToRenderQueue(entity2);
            renderer->addToRenderQueue(entity3);
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
