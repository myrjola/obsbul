#include "defaultfirstpersoncontroller.h"

const float MOVEMENT_SPEED = 0.15;
const float TURN_RATE = 0.01;

DefaultFirstPersonController::DefaultFirstPersonController(
    const shared_ptr< Entity > controllable)
:
m_controllable(controllable)
{}

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
            if (keyevent.Control) {
                m_controllable->m_velocity_local.x = -MOVEMENT_SPEED;
            } else {
                m_controllable->m_delta_orientation.yaw -= TURN_RATE;
            }
            break;
        case sf::Key::D:
            if (keyevent.Control) {
                m_controllable->m_velocity_local.x = MOVEMENT_SPEED;
            } else {
                m_controllable->m_delta_orientation.yaw += TURN_RATE;
            }
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
    if (keyevent.Control) {
        float sign = 0.0f;
        if (m_controllable->m_velocity_local.x != 0.0f) {
            (m_controllable->m_velocity_local.x > 0.0f) ? sign = 1.0f : sign = -1.0f;
        }
        m_controllable->m_delta_orientation.yaw = sign * TURN_RATE;
        m_controllable->m_velocity_local.x = 0.0f;
    }
    switch (keyevent.Code) {
        case sf::Key::W:
            m_controllable->m_velocity_local.z += MOVEMENT_SPEED;
            break;
        case sf::Key::S:
            m_controllable->m_velocity_local.z -= MOVEMENT_SPEED;
            break;
        case sf::Key::A:
            if (m_controllable->m_delta_orientation.yaw < 0.0)
                m_controllable->m_delta_orientation.yaw += TURN_RATE;
            break;
        case sf::Key::D:
            if (m_controllable->m_delta_orientation.yaw > 0.0)
                m_controllable->m_delta_orientation.yaw -= TURN_RATE;
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


void DefaultFirstPersonController::mouseMoved(sf::Event::MouseMoveEvent event)
{}
