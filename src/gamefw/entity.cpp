
#include "entity.h"

using namespace gamefw;

Entity::Entity()
:
m_name(new string("")), m_desc(new string("")),
m_position(glm::vec3(0.0f, 0.0f, 0.0f))
{
	m_orientation.yaw = 0.0f;
	m_orientation.pitch = 0.0f;
	m_orientation.roll = 0.0f;
}

Entity::~Entity()
{
}

// Entity::Entity(const gamefw::Entity& entity)
// {
// }

const string Entity::getDesc() const
{
    return *m_desc;
}

const string Entity::getName() const
{
    return *m_name;
}

void Entity::setRenderJob(shared_ptr< RenderJob > renderjob)
{
    m_renderjob = renderjob;
}

shared_ptr< RenderJob > Entity::getRenderJob() const
{
    return m_renderjob;
}

void Entity::setDesc(const char* desc)
{
    shared_ptr<string> temp(new string(desc));
    m_desc.swap(temp);
}

void Entity::setName(const char* name)
{
    shared_ptr<string> temp(new string(name));
    m_name.swap(temp);
}






