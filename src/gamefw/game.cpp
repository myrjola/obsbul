#include "../common.h"

#include "game.h"

#include <SFML/Graphics.hpp>

using namespace gamefw;

enum Windows {
    MAIN
};

Game::Game(const uint display_width, const uint display_height) :
        m_main_window_context(24, 8, 0, 3, 3)
{
    m_main_window.Create(sf::VideoMode(display_width, display_height,
                                       24), "Test", sf::Style::Default,
                         m_main_window_context);
    m_main_window.SetActive();
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
    while (m_main_window.GetEvent(event)) {
        if (event.Type == sf::Event::Closed) {
            m_main_window.Close();
            return 0;
        }
        if (event.Type == sf::Event::KeyPressed) {
            switch (event.Key.Code) {
                case (sf::Key::R):
                    Locator::getShaderFactory().reloadShaders();
                    break;
            }
        }
    }
    return 1;
}
