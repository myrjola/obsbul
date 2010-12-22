#ifndef IGAMEWORLD_H
#define IGAMEWORLD_H

#include "entity.h"

namespace gamefw {

class IGameWorld
{
public:
    virtual void addEntity(shared_ptr<Entity> entity) = 0;
    virtual void addEntities(const vector<shared_ptr<Entity> >& entities) = 0;
    virtual void update() = 0;
    virtual shared_ptr<vector< shared_ptr<Entity> > > getEntityList() = 0;
};

}

#endif // IGAMEWORLD_H
