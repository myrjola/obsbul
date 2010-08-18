#ifndef ENTITY_H
#define ENTITY_H

#include "../common.h"

namespace gamefw {

class RenderJob;

class Entity
{
public:
    Entity();

    ~Entity();
     
    /// Getters and Setters
    string getName();
    void setName(const char* name);
    
    string getDesc();
    void setDesc(const char* desc);

    shared_ptr<RenderJob> getRenderJob();
    void setRenderJob(shared_ptr<RenderJob> renderjob);
    
private:
    shared_ptr<string> m_name;
    shared_ptr<string> m_desc;
    shared_ptr<RenderJob> m_renderjob;
};

}

#endif // ENTITY_H
