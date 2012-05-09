#include "defaultfirstpersongamestate.h"

DefaultFirstPersonGameState::DefaultFirstPersonGameState(gamefw::Game* parent,
                                                         shared_ptr<Entity> controllable)
:
m_parent(parent),
m_controller(controllable)
{
    sf::Window* main_window = m_parent->getMainWindow();
    const sf::Vector2u windowsize = main_window->getSize();
    m_window_middle_x = windowsize.x / 2;
    m_window_middle_y = windowsize.y / 2;
}

DefaultFirstPersonGameState::~DefaultFirstPersonGameState()
{

}

void DefaultFirstPersonGameState::changeControlledEntity(shared_ptr< Entity > controllable)
{
    m_controller = DefaultFirstPersonController(controllable);
}


UpdateStatus DefaultFirstPersonGameState::update()
{
    sf::Window* main_window = m_parent->getMainWindow();
    sf::Event event;
    while (main_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            return UPDATE_QUIT;
        } else if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case (sf::Keyboard::R):
                    LOG(logERROR) << "Reloading shaders.";
                    gamefw::Locator::getShaderFactory().reloadShaders();
                    break;
                case (sf::Keyboard::Escape):
                    return UPDATE_QUIT;
                default:
                    m_controller.keyPressed(event.key);
            }
        } else if (event.type == sf::Event::KeyReleased) {
            m_controller.keyReleased(event.key);
        }
    }
    
    sf::Vector2i mousepos = sf::Mouse::getPosition(*main_window);
    int x = mousepos.x;
    int y = mousepos.y;
    m_controller.mouseMoved(x, y);

    // Center mouse if outside of window.
    if (glm::abs(x - m_window_middle_x) > 100 ||
        glm::abs(y - m_window_middle_y) > 100) {
        m_controller.newMousePosition(m_window_middle_x, m_window_middle_y);
        const sf::Vector2i mousepos = sf::Vector2i(m_window_middle_x, m_window_middle_y);
        sf::Mouse::setPosition(mousepos, *main_window);
    }
    return UPDATE_NORMAL;
}
