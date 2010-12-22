#include "simplegameworld.h"

SimpleGameWorld::SimpleGameWorld()
:
m_entity_list(new vector<shared_ptr<gamefw::Entity> >())
{

}

void SimpleGameWorld::addEntity(shared_ptr< gamefw::Entity > entity)
{
    m_entity_list->push_back(entity);
}

void SimpleGameWorld::addEntities(const std::vector<shared_ptr<gamefw::Entity> >& entities)
{
    foreach(shared_ptr<gamefw::Entity> entity, entities) {
        addEntity(entity);
    }
}

void SimpleGameWorld::update()
{
    foreach(shared_ptr<gamefw::Entity> entity, *m_entity_list) {
        entity->m_position.x += entity->m_velocity_local.x *
                        glm::cos(glm::radians(entity->m_orientation.x)) -
                        entity->m_velocity_local.z *
                        glm::sin(glm::radians(entity->m_orientation.x));
        entity->m_position.z += entity->m_velocity_local.x *
                        glm::sin(glm::radians(entity->m_orientation.x)) +
                        entity->m_velocity_local.z *
                        glm::cos(glm::radians(entity->m_orientation.x));
        entity->m_position.y += entity->m_velocity_local.y;
        
        entity->m_orientation += entity->m_angular_velocity;
    }
}

shared_ptr< vector< shared_ptr< gamefw::Entity > > > SimpleGameWorld::getEntityList()
{
    return m_entity_list;
}
