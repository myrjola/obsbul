#include "levelfile.h"

#include "locator.h"
#include "entity.h"

const char* gamefw::LevelLoadError::what() const throw()
{
    return "Error when loading level.";
}

gamefw::LevelFile::LevelFile(string path)
{
    TiXmlDocument levelfile(path);
    if (!levelfile.LoadFile()) { // If error when loading file.
        LOG(logERROR) << "Error when loading entity file " << path <<
        "\nError at line " << levelfile.ErrorRow() <<
        "\nError description: " << levelfile.ErrorDesc();
        throw LevelLoadError();
    }
    TiXmlHandle levelhandle = TiXmlHandle(&levelfile).FirstChild("level");
    
    // Create and add entities.
    int num_entities = 0;
    TiXmlElement* entityelement = levelhandle.Child("entity", num_entities++).ToElement();
    while (entityelement) {
        shared_ptr<Entity> entity = createEntity(*entityelement);
        m_entities.push_back(entity);
        entityelement = levelhandle.Child("entity", num_entities++).ToElement();
    }

    // Create and add pointlights.
    int num_pointlights = 0;
    TiXmlElement* pointlightelement = levelhandle.Child("pointlight", num_pointlights++).ToElement();
    while (pointlightelement) {
        shared_ptr<PointLight> pointlight = createPointLight(*pointlightelement);
        m_pointlights.push_back(pointlight);
        pointlightelement = levelhandle.Child("pointlight", num_pointlights++).ToElement();
    }
}

shared_ptr< gamefw::Entity > gamefw::LevelFile::createEntity(const TiXmlElement& entityelement)
{
    string name(entityelement.Attribute("name"));

    // Load entity if it isn't.
    if (m_loaded_entities.find(name) == m_loaded_entities.end()) {
        m_loaded_entities[name] = Locator::getFileService().createEntity(name);
    }
    shared_ptr<Entity> entity(new Entity(*m_loaded_entities.at(name)));

    // Assign position.
    string pos_str(entityelement.FirstChild("position")->ToElement()->GetText());
    istringstream position(pos_str);
    float x, y, z;
    position >> x >> y >> z;
    entity->m_position = glm::vec3(x, y, z);
    
    // Assign orientation.
    string orientation_str(entityelement.FirstChild("orientation")->ToElement()->GetText());
    istringstream orientation(orientation_str);
    float yaw, pitch, roll;
    orientation >> yaw >> pitch >> roll;
    entity->m_orientation = glm::vec3(yaw, pitch, roll);

    return entity;
}

shared_ptr< gamefw::PointLight > gamefw::LevelFile::createPointLight(const TiXmlElement& pointlightelement)
{
    shared_ptr< Entity > entity = createEntity(pointlightelement);
    shared_ptr< PointLight > pointlight(new PointLight(*entity));
    
    // Assign color.
    string col_str(pointlightelement.FirstChild("color")->ToElement()->GetText());
    istringstream color(col_str);
    float r, g, b;
    color >> r >> g >> b;
    pointlight->m_color = glm::vec3(r, g, b);

    // Assign intensity.
    string intensity_str(pointlightelement.FirstChild("intensity")->ToElement()->GetText());
    istringstream intensity(intensity_str);
    intensity >> pointlight->m_intensity;

    return pointlight;
}