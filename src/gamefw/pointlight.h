#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "gamefw.h"

namespace gamefw {
    
class PointLight : public Entity
{

public:
    PointLight();
    PointLight(const Entity& entity);

    glm::vec3 m_color;
};
    
}


#endif // POINTLIGHT_H
