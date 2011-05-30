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

    FileService fileservice(OGL_3_3);
    Locator::registerFileService(fileservice);
    ShaderFactory shaderfactory(OGL_3_3);
    Locator::registerShaderFactory(shaderfactory);
    
    uint width = 1024, height = 768;
    Game game(width, height, OGL_3_3);

    shared_ptr<Renderer> renderer = game.getRenderer();

    sf::Clock clock;

    sf::Window* main_window = game.getMainWindow();

    shared_ptr<PointLight> camera(new PointLight());
    camera->m_intensity = 5.0f;
    shared_ptr<IGameState> gamestate(new DefaultFirstPersonGameState(&game, camera));
    game.changeGameState(gamestate);

    renderer->changeCamera(camera);

    SimpleGameWorld gameworld;
    shared_ptr<LevelFile> level = Locator::getFileService().loadLevelFile("lvl01");
    vector<shared_ptr< Entity > > entitylist = level->m_entities;
    vector<shared_ptr< PointLight > > pointlightlist = level->m_pointlights;
    gameworld.addEntities(entitylist);
    gameworld.addEntity(camera);
    

    while (true) {
        float time_since_draw = clock.GetElapsedTime();
        if (time_since_draw >= 1.0f/60) {
            gameworld.update();
            clock.Reset();
            foreach(shared_ptr<Entity> entity, entitylist) {
                renderer->addToRenderQueue(entity);
            }
            
            renderer->addToPointLightQueue(camera);
            foreach(shared_ptr<PointLight> pointlight, pointlightlist) {
                renderer->addToPointLightQueue(pointlight);
                renderer->addToRenderQueue(pointlight);
            }
                
            if (game.update() == UPDATE_QUIT) { // If window closing.
                break;
            }
        }
    }

    return 0;
}
