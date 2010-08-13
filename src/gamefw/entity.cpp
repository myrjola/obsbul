/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/

#include "entity.h"

using namespace gamefw;

Entity::Entity() : m_name(new string("")), m_desc(new string(""))
{
}

Entity::~Entity()
{
}

string Entity::getDesc()
{
    return *m_desc;
}

string Entity::getName()
{
    return *m_name;
}

void Entity::setRenderJob(shared_ptr< RenderJob > renderjob)
{
    m_renderjob = renderjob;
}

shared_ptr< RenderJob > Entity::getRenderJob()
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






