
#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include <queue>

namespace gamefw {

class Entity;
class RenderJob;


class RenderContext
{
public:

private:
    std::queue<Entity*> m_render_queue;
public:
    void addToRenderQueue(Entity& entity);
    void render();
};

}

#endif // RENDERCONTEXT_H
