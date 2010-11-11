#include "defaultfirstpersoncontroller.h"

DefaultFirstPersonController::DefaultFirstPersonController(
    shared_ptr< Entity > controllable)
:
m_controllable(controllable)
{}

void DefaultFirstPersonController::keyPressed(sf::Key::Code keycode)
{
}

void DefaultFirstPersonController::keyReleased(sf::Key::Code keycode)
{
}


void DefaultFirstPersonController::mouseMoved(sf::Event::MouseMoveEvent event)
{
    m_controllable->m_orientation.yaw += event.X;
    m_controllable->m_orientation.pitch += event.Y;
}
