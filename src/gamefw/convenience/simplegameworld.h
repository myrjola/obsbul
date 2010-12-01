#ifndef SIMPLEGAMEWORLD_H
#define SIMPLEGAMEWORLD_H

#include "../../common.h"
#include "../entity.h"

class SimpleGameWorld
{
public:
    void addEntity(shared_ptr<gamefw::Entity> entity);
    void update();

private:
    vector<shared_ptr<gamefw::Entity> > m_entity_list;
};

#endif // SIMPLEGAMEWORLD_H
