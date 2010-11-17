#ifndef ENTITY_H
#define ENTITY_H

#include "../common.h"

namespace gamefw {

class RenderJob;

/**
 * @brief Game object.
 */
class Entity
{
public:
    Entity();

    ~Entity();

//     Entity(const Entity& entity);
     
    /// Getters and Setters
    const std::string getName() const;
    void setName(const char* name);
    
    const std::string getDesc() const;
    void setDesc(const char* desc);

    shared_ptr<RenderJob> getRenderJob() const;
    void setRenderJob(shared_ptr<RenderJob> renderjob);

    /**
     * @brief World space position.
     **/
    glm::vec3 m_position;
    
    /**
     * @brief World space orientation in degrees.
     **/
    struct {
        float yaw, pitch, roll;
    } m_orientation;

    /// Movement.
    glm::vec3 m_velocity_local;

private:
    shared_ptr<string> m_name;
    shared_ptr<string> m_desc;
    shared_ptr<RenderJob> m_renderjob;
};

}

#endif // ENTITY_H
