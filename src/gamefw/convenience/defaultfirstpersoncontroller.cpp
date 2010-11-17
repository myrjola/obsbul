#include "defaultfirstpersoncontroller.h"

#include "SFML/System.hpp"

const float MOVEMENT_SPEED = 0.15;

DefaultFirstPersonController::DefaultFirstPersonController(
    const shared_ptr< Entity > controllable)
:
m_controllable(controllable),
m_last_mouse_x(0),
m_last_mouse_y(0)
{
}

void DefaultFirstPersonController::keyPressed(sf::Event::KeyEvent& keyevent)
{
    switch (keyevent.Code) {
        case sf::Key::W:
            m_controllable->m_velocity_local.z -= MOVEMENT_SPEED;
            break;
        case sf::Key::S:
            m_controllable->m_velocity_local.z += MOVEMENT_SPEED;
            break;
        case sf::Key::A:
            m_controllable->m_velocity_local.x = -MOVEMENT_SPEED;
            break;
        case sf::Key::D:
            m_controllable->m_velocity_local.x = MOVEMENT_SPEED;
            break;
        case sf::Key::Space:
            m_controllable->m_velocity_local.y += MOVEMENT_SPEED;
            break;
        case sf::Key::C:
            m_controllable->m_velocity_local.y -= MOVEMENT_SPEED;
            break;
        default:
            break;
    }
}

void DefaultFirstPersonController::keyReleased(sf::Event::KeyEvent& keyevent)
{
    switch (keyevent.Code) {
        case sf::Key::W:
            m_controllable->m_velocity_local.z += MOVEMENT_SPEED;
            break;
        case sf::Key::S:
            m_controllable->m_velocity_local.z -= MOVEMENT_SPEED;
            break;
        case sf::Key::A:
            m_controllable->m_velocity_local.x += MOVEMENT_SPEED;
            break;
        case sf::Key::D:
            m_controllable->m_velocity_local.x -= MOVEMENT_SPEED;
            break;
        case sf::Key::Space:
            m_controllable->m_velocity_local.y -= MOVEMENT_SPEED;
            break;
        case sf::Key::C:
            m_controllable->m_velocity_local.y += MOVEMENT_SPEED;
            break;
        default:
            break;
    }
}


void DefaultFirstPersonController::mouseMoved(const int x, const int y,
                                              const sf::Input& input_state)
{
    m_controllable->m_orientation.yaw += x - m_last_mouse_x;
    m_controllable->m_orientation.pitch -= y - m_last_mouse_y;
    newMousePosition(x, y);
}

void DefaultFirstPersonController::newMousePosition(const int width,
                                                    const int height)
{
    m_last_mouse_x = width;
    m_last_mouse_y = height;
}

