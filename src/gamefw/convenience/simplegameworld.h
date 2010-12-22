#ifndef SIMPLEGAMEWORLD_H
#define SIMPLEGAMEWORLD_H

#include "../../common.h"
#include "../entity.h"
#include "../igameworld.h"

class SimpleGameWorld : public gamefw::IGameWorld
{
public:
    SimpleGameWorld();
    virtual void addEntity(shared_ptr< gamefw::Entity > entity);
    virtual void addEntities(const std::vector< shared_ptr< gamefw::Entity > >& entities);
    virtual void update();
    virtual shared_ptr< vector< shared_ptr< gamefw::Entity > > > getEntityList();
    
private:
    shared_ptr<vector<shared_ptr<gamefw::Entity> > > m_entity_list;
};

#endif // SIMPLEGAMEWORLD_H
