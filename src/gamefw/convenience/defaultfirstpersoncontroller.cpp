#include "defaultfirstpersoncontroller.h"

#include "SFML/System.hpp"
#include "SFML/Window/Keyboard.hpp"

const float MOVEMENT_SPEED = 0.15f;

DefaultFirstPersonController::DefaultFirstPersonController(
    shared_ptr< Entity > controllable)
:
m_controllable(controllable),
m_last_mouse_x(0),
m_last_mouse_y(0)
{
}

void DefaultFirstPersonController::keyPressed(sf::Event::KeyEvent& keyevent)
{
    switch (keyevent.code) {
        case sf::Keyboard::W:
            m_controllable->m_velocity_local.z -= MOVEMENT_SPEED;
            break;
        case sf::Keyboard::S:
            m_controllable->m_velocity_local.z += MOVEMENT_SPEED;
            break;
        case sf::Keyboard::A:
            m_controllable->m_velocity_local.x = -MOVEMENT_SPEED;
            break;
        case sf::Keyboard::D:
            m_controllable->m_velocity_local.x = MOVEMENT_SPEED;
            break;
        case sf::Keyboard::Space:
            m_controllable->m_velocity_local.y += MOVEMENT_SPEED;
            break;
        case sf::Keyboard::C:
            m_controllable->m_velocity_local.y -= MOVEMENT_SPEED;
            break;
        default:
            break;
    }
}

void DefaultFirstPersonController::keyReleased(sf::Event::KeyEvent& keyevent)
{
    switch (keyevent.code) {
        case sf::Keyboard::W:
            m_controllable->m_velocity_local.z += MOVEMENT_SPEED;
            break;
        case sf::Keyboard::S:
            m_controllable->m_velocity_local.z -= MOVEMENT_SPEED;
            break;
        case sf::Keyboard::A:
            m_controllable->m_velocity_local.x += MOVEMENT_SPEED;
            break;
        case sf::Keyboard::D:
            m_controllable->m_velocity_local.x -= MOVEMENT_SPEED;
            break;
        case sf::Keyboard::Space:
            m_controllable->m_velocity_local.y -= MOVEMENT_SPEED;
            break;
        case sf::Keyboard::C:
            m_controllable->m_velocity_local.y += MOVEMENT_SPEED;
            break;
        default:
            break;
    }
}


void DefaultFirstPersonController::mouseMoved(const int x, const int y)
{
    m_controllable->m_orientation.x += (x - m_last_mouse_x) / 5.0f;
    m_controllable->m_orientation.y -= (y - m_last_mouse_y) / 5.0f;
    newMousePosition(x, y);
}

void DefaultFirstPersonController::newMousePosition(const int width,
                                                    const int height)
{
    m_last_mouse_x = width;
    m_last_mouse_y = height;
}

