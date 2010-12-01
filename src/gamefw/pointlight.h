#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "entity.h"

namespace gamefw {

class PointLight
:
public Entity
{

public:
    PointLight();
    PointLight(const Entity& entity);
    
    virtual ~PointLight();

    glm::vec3 m_color;
    float m_intensity;
};
    
}


#endif // POINTLIGHT_H
