#include "defaultfirstpersongamestate.h"

DefaultFirstPersonGameState::DefaultFirstPersonGameState(gamefw::Game* parent,
                                                         shared_ptr<Entity> controllable)
:
m_parent(parent),
m_controller(controllable)
{
    sf::Window* main_window = m_parent->getMainWindow();
    m_window_middle_x = main_window->GetWidth() / 2;
    m_window_middle_y = main_window->GetHeight() / 2;
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
    const sf::Input& input_state = main_window->GetInput();
    while (main_window->GetEvent(event)) {
        if (event.Type == sf::Event::Closed) {
            return UpdateStatus::UPDATE_QUIT;
        } else if (event.Type == sf::Event::KeyPressed) {
            switch (event.Key.Code) {
                case (sf::Key::R):
                    gamefw::Locator::getShaderFactory().reloadShaders();
                    break;
                case (sf::Key::Escape):
                    return UpdateStatus::UPDATE_QUIT;
                default:
                    m_controller.keyPressed(event.Key);
            }
        } else if (event.Type == sf::Event::KeyReleased) {
            m_controller.keyReleased(event.Key);
        }
    }

    int x = input_state.GetMouseX();
    int y = input_state.GetMouseY();
    m_controller.mouseMoved(x, y, input_state);

    // Center mouse if outside of window.
    if (glm::abs(x - m_window_middle_x) > 100 ||
        glm::abs(y - m_window_middle_y) > 100) {
        m_controller.newMousePosition(m_window_middle_x, m_window_middle_y);
        main_window->SetCursorPosition(m_window_middle_x, m_window_middle_y);
    }
    return UpdateStatus::UPDATE_NORMAL;
}
