#include "../../common.h"

#include "../gamefw.h"
#include "../igamestate.h"

#include "../convenience/defaultfirstpersoncontroller.h"
#include "../convenience/defaultfirstpersongamestate.h"
#include "../convenience/simplegameworld.h"

#include <physfs.h>

using namespace gamefw;

int main(int argc, char* argv[])
{
    PHYSFS_init(argv[0]);

    int status= glewInit();
    if (GLEW_OK != status) {
        LOG(logERROR) << "Error:" << glewGetErrorString(status) << "\n";
    }
    
    FileService* fileservice = new FileService();
    Locator::registerFileService(*fileservice);
    ShaderFactory* shaderfactory = new ShaderFactory();
    Locator::registerShaderFactory(*shaderfactory);
    
    uint width = 1024, height = 768;
    Game game(width, height);

    shared_ptr<Entity> entity = Locator::getFileService().createEntity("flatsmooth");
    shared_ptr<Entity> entity2 = Locator::getFileService().createEntity("lightball");
    shared_ptr<Entity> entity3 = Locator::getFileService().createEntity("sphere");
    shared_ptr<PointLight> light(new PointLight(*entity2));

    entity->m_position = glm::vec3(5.0f, 0.0f, -15.0f);
    light->m_position = glm::vec3(5.0f, 0.0f, -5.0f);
    entity3->m_position = glm::vec3(0.0f, 0.0f, -5.0f);

    shared_ptr<Renderer> renderer = game.getRenderer();

    sf::Clock clock;

    sf::Window* main_window = game.getMainWindow();

    PointLight* p_camera = new PointLight();
    p_camera->m_intensity = 1.0f;
    shared_ptr<PointLight> camera(p_camera);
    shared_ptr<IGameState> gamestate(new DefaultFirstPersonGameState(&game, camera));
    game.changeGameState(gamestate);

    renderer->changeCamera(camera);

    SimpleGameWorld gameworld;
    gameworld.addEntity(camera);

    float timer = 0.0f;
    while (true) {
        float time_since_draw = clock.GetElapsedTime();
        if (time_since_draw >= 1.0f/60) {
            gameworld.update();
            clock.Reset();
            
            light->m_position.x = 14.0f * glm::sin(timer);
            light->m_position.y = 14.0f * glm::cos(timer);
            timer += 0.01f;
            entity->m_orientation.x += 0.01f;
            light->m_orientation.y += 0.03f;
            entity3->m_orientation.x += 0.02f;
            entity3->m_orientation.y += 0.005f;
            renderer->addToRenderQueue(entity);
            renderer->addToRenderQueue(light);
            renderer->addToRenderQueue(entity3);
            renderer->addToPointLightQueue(light);
            renderer->addToPointLightQueue(camera);
            if (game.update() == UpdateStatus::UPDATE_QUIT) { // If window closing.
                break;
            }
        }
    }
    delete fileservice;
    delete shaderfactory;

    return 0;
}
