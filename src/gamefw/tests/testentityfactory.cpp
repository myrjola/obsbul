#include <UnitTest++.h>

#include "../gamefw.h"

using namespace gamefw;

struct EntityFactoryFixture
{
    EntityFactoryFixture()
    {
        FileService* fileservice = new FileService();
        Locator::registerFileService(*fileservice);
        ShaderFactory* shaderfactory = new ShaderFactory();
        Locator::registerShaderFactory(*shaderfactory);
    }

    ~EntityFactoryFixture()
    {
        FileService& fileservice = Locator::getFileService();
        delete &fileservice;
        ShaderFactory& shaderfactory = Locator::getShaderFactory();
        delete &shaderfactory;
    }
};

TEST_FIXTURE(EntityFactoryFixture, TestCreateEntity)
{
    Entity entity = Locator::getFileService().createEntity("sphere");
    CHECK_EQUAL("Sphere", entity.getName());
    CHECK_EQUAL("A sphere using albedo and normal textures with phong shading",
                entity.getDesc());
}
