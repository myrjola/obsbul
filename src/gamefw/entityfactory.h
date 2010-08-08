/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/

#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "../common.h"

#define TIXML_USE_STL
#include <tinyxml.h>

#include "gamefw.h"

namespace gamefw {

class Entity;

/**
 * @brief Thrown when EntityFactory fails in the creation of an Entity.
 */
class EntityCreationError: public exception
{
    /**
     * @return Description of exception.
     */
    virtual const char* what() const throw();
};

/**
 * @brief Takes care of Entity creation.
 *
 * Is used inside FileService to serve entities to the GameWorld. Parses a xml-
 * file and initializes an Entity according to it.
 */
class EntityFactory
{
public:
    /**
     * Constructs an Entity from it's file. The format is simple XML.
     *
     * @throw EntityCreationError When the creation fails.
     *
     * @param path The absolute path to the Entity's file.
     * @return Reference to the constructed Entity.
     */
    Entity& createEntity(string path);

private:
    void loadModel(string& path, shared_ptr<RenderJob> renderjob);
    
    TiXmlDocument* m_current_entityfile;
};

}

#endif // ENTITYFACTORY_H
