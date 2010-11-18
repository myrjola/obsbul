#include "pointlight.h"

gamefw::PointLight::PointLight()
:
Entity(),
color(1.0, 1.0, 1.0)
{
}

gamefw::PointLight::PointLight(const gamefw::Entity& entity)
:
Entity(entity),
color(1.0, 1.0, 1.0)
{

}

