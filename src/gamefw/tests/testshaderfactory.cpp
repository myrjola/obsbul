#include <UnitTest++.h>

#include <set>
#include "../gamefw.h"

struct ShaderFactoryFixture
{
    ShaderFactoryFixture()
    {
        FileService* fileservice = new FileService();
        Locator::RegisterFileService(*fileservice);
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
    d2.insert("DUMMY2");
    
    ShaderProgram& p1 = factory->makeShader(d1);
    ShaderProgram& also_p1 = factory->makeShader(d1);
    ShaderProgram& p2 = factory->makeShader(d2);
    
    CHECK_EQUAL(p1.getProgramID(), also_p1.getProgramID());

    CHECK(p1.getProgramID() != p2.getProgramID());
}

