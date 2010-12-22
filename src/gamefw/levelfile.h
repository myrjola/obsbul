#ifndef LEVELFILE_H
#define LEVELFILE_H

#include "../common.h"

#define TIXML_USE_STL
#include <tinyxml.h>
#include <map>

#include "igameworld.h"
#include "pointlight.h"

namespace gamefw {
    
/**
 * @brief Thrown when EntityFactory fails in the creation of an Entity.
 */
class LevelLoadError: public exception
{
    /**
     * @return Description of exception.
     */
    virtual const char* what() const throw();
};

/**
 * @brief Representation of a game level.
 **/
class LevelFile
{
public:
    /**
     * @brief Loads a level file.
     *
     * @param path ditto.
     * @return void
     **/
    LevelFile(string path);

    vector<shared_ptr<Entity> > m_entities;
    vector<shared_ptr<PointLight> > m_pointlights;
    map<string, shared_ptr<Entity> > m_loaded_entities;
    
private:
    shared_ptr<Entity> createEntity(const TiXmlElement& entityelement);
    shared_ptr<PointLight> createPointLight(const TiXmlElement& pointlightelement);
};

}

#endif // LEVELFILE_H
