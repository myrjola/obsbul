#include <UnitTest++.h>

#include <set>
#include "../gamefw.h"

using namespace gamefw;

struct ShaderFactoryFixture
{
    ShaderFactoryFixture()
    {
        FileService* fileservice = new FileService();
        Locator::registerFileService(*fileservice);
        factory = new ShaderFactory();
        
    }

    ~ShaderFactoryFixture()
    {
        delete factory;
        FileService& fileservice = Locator::getFileService();
        delete &fileservice;
    }

    ShaderFactory* factory;
};

TEST_FIXTURE(ShaderFactoryFixture, TestMakeShader)
{
    set<string> d1;
    d1.insert("DUMMY1");
    d1.insert("DUMMY2");
    set<string> d2;
    d2.insert("DUMMY3");
    
    ShaderProgram& p1 = factory->makeShader(d1);
    ShaderProgram& also_p1 = factory->makeShader(d1);
    ShaderProgram& p2 = factory->makeShader(d2);
    
    CHECK_EQUAL(p1.getProgramID(), also_p1.getProgramID());

    CHECK(p1.getProgramID() != p2.getProgramID());
}

TEST_FIXTURE(ShaderFactoryFixture, TestMakeShaderEmptyDefines) {
    set<string> empty_set;
    CHECK_THROW(factory->makeShader(empty_set), ShaderProgramCreationError);
}
