#include "../common.h"

#include "game.h"

#include <SFML/Graphics.hpp>

using namespace gamefw;

enum Windows {
    MAIN
};

Game::Game(const uint display_width, const uint display_height)
:
m_main_window_context(24, 8, 0, 3, 3),
m_controller(NULL),
m_window_middle_x(display_width / 2),
m_window_middle_y(display_height / 2)
{
    m_main_window.Create(sf::VideoMode(display_width, display_height,
                                       24), "Test", sf::Style::Default,
                         m_main_window_context);
    m_main_window.SetActive();
    m_main_window.EnableKeyRepeat(false);
    m_main_window.ShowMouseCursor(false);
    m_main_window.UseVerticalSync(true);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

Game::~Game()
{
    m_main_window.Close();
}

sf::Window* Game::getMainWindow()
{
    return &m_main_window;
}


int Game::update()
{
    sf::Event event;
    const sf::Input& input_state = m_main_window.GetInput();
    while (m_main_window.GetEvent(event)) {
        if (event.Type == sf::Event::Closed) {
            m_main_window.Close();
            return 0;
        } else if (event.Type == sf::Event::KeyPressed) {
            switch (event.Key.Code) {
                case (sf::Key::R):
                    Locator::getShaderFactory().reloadShaders();
                    break;
                case (sf::Key::Escape):
                    m_main_window.Close();
                    return 0;
                default:
                    m_controller->keyPressed(event.Key);
            }
        } else if (event.Type == sf::Event::KeyReleased) {
            m_controller->keyReleased(event.Key);
        }
    }

    int x = input_state.GetMouseX();
    int y = input_state.GetMouseY();
    m_controller->mouseMoved(x, y, input_state);
    
    // Center mouse if outside of window.
    if (glm::abs(x - m_window_middle_x) > 100 ||
        glm::abs(y - m_window_middle_y) > 100) {
        m_controller->newMousePosition(m_window_middle_x, m_window_middle_y);
        m_main_window.SetCursorPosition(m_window_middle_x, m_window_middle_y);
    }

    return 1;
}

void gamefw::Game::changeController(IController* controller)
{
    m_controller = controller;
}
