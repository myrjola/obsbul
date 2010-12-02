
#include "entity.h"

using namespace gamefw;

Entity::Entity()
:
m_name(new string("")), m_desc(new string("")),
m_position(0.0f, 0.0f, 0.0f),
m_velocity_local(0.0f, 0.0f, 0.0f),
m_orientation(0.0f, 0.0f, 0.0f)
{
}

Entity::~Entity()
{
}

shared_ptr<string> Entity::getDesc() const
{
    return m_desc;
}

shared_ptr<string> Entity::getName() const
{
    return m_name;
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
    m_desc = shared_ptr<string>(new string(desc));
}

void Entity::setName(const char* name)
{
    m_name = shared_ptr<string>(new string(name));
}






