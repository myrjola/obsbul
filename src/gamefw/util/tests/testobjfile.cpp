#include <UnitTest++.h>

#include "../../../common.h"
#include "../../gamefw.h"
#include "../objfile.h"

#include <physfs.h>
#include <boost/concept_check.hpp>

using namespace gamefw;

struct ObjFileFixture
{
    ObjFileFixture()
    {
        FileService* fileservice = new FileService();
        Locator::registerFileService(*fileservice);
    }

    ~ObjFileFixture()
    {
        FileService& fileservice = Locator::getFileService();
        delete &fileservice;
    }
};

TEST_FIXTURE(ObjFileFixture, TestLoadSimpleObj)
{
    string separator = Locator::getFileService().getDirSeparator();
    string path = Locator::getFileService().getRealPath("src/gamefw/util/tests/simple.obj");
    ObjFile model(path);
    CHECK_EQUAL(model.getNumMaterials(), 2); // +1 for the default mtl.
    CHECK_EQUAL(model.getNumNormals(), 1);
    CHECK_EQUAL(model.getNumPositions(), 3);
    CHECK_EQUAL(model.getNumTexcoords(), 3);
    CHECK_EQUAL(model.getNumTriangles(), 1);

    float positions[] = {1.0f, -1.0f, 1.0f,
                         -1.0f, -1.0f, 1.0f,
                         -1.0f, -1.0f, -1.0f};

    float normals[] = {0.0f, -1.0f, 0.0f};
    
    float texcoords[] = {0.0f, 0.5f,
                         0.5f, 0.0f,
                         1.0f, 0.5f};

    CHECK_ARRAY_CLOSE(model.getPositions(), positions, 9, 0.00);
    CHECK_ARRAY_CLOSE(model.getNormals(), normals, 3, 0.00);
    CHECK_ARRAY_CLOSE(model.getTexCoords(), texcoords, 6, 0.00);
    
    t_obj_triangle triangle1 = {
        {1, 2, 3},
        {1, 2, 3},
        {1, 1, 1},
        1
    };

    t_obj_triangle triangle2 = *model.getTriangles();
    
    CHECK_ARRAY_EQUAL(triangle1.pindices, triangle2.pindices, 3);
    CHECK_ARRAY_EQUAL(triangle1.nindices, triangle2.nindices, 3);
    CHECK_ARRAY_EQUAL(triangle1.tindices, triangle2.tindices, 3);
    CHECK_EQUAL(triangle1.material, triangle2.material);

    t_obj_mtl material1 = {
        {0.64f, 0.64f, 0.64f, 1.0f},
        {0.5f, 0.5f, 0.5f, 1.0f},
        128.0,
        {0.0f, 0.0f, 0.0f}
    };

    t_obj_mtl material2 = model.getMaterials()[1];
    
    CHECK_ARRAY_CLOSE(material1.diffuse, material2.diffuse, 4, 0.00);
    CHECK_ARRAY_CLOSE(material1.specular, material2.specular, 4, 0.00);
    CHECK_CLOSE(material1.shininess, material2.shininess, 0.00);
}

int main(int argc, char* argv[])
{
    PHYSFS_init(argv[0]);

    return UnitTest::RunAllTests();
    return 0;
}