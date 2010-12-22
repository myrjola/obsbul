#include "pointlight.h"

gamefw::PointLight::PointLight()
        :
        Entity(),
        m_color(1.0, 1.0, 1.0),
        m_intensity(100.0)
{
}

gamefw::PointLight::PointLight(const gamefw::Entity& entity)
        :
        Entity(entity),
        m_color(1.0, 1.0, 1.0),
        m_intensity(100.0)
{

}

gamefw::PointLight::~PointLight()
{

}


