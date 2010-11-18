#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "gamefw.h"

namespace gamefw {
    
class PointLight : public Entity
{

public:
    PointLight();
    PointLight(const Entity& entity);
    
    virtual ~PointLight();

    glm::vec3 m_color;
    GLfloat m_intensity;
};
    
}


#endif // POINTLIGHT_H
