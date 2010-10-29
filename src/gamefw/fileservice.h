
#ifndef FILESERVICE_H
#define FILESERVICE_H

#include "../common.h"

#ifndef PROJECT_NAME
    #define PROJECT_NAME "ObscureBulldozer"
#endif


#include <GL/gl.h>

class fipImage;

namespace gamefw {

class EntityFactory;

class Entity;

/**
 * @brief Exception thrown when FileService can't find a file
 */
class FileNotFoundException: public exception
{
public:
    /**
     * @return Description of exception.
     */
    virtual const char* what() const throw();
};

/**
 * @brief Provides centralized handling of files.
 *
 * Is used for most file management in the engine. For example loads shader
 * sources into buffer or creates an OpenGL texture object of a given image name.
 * Uses the freeimage library for images.
 *
 * PhysFS is used under the hood for a virtual file system. The projects' root
 * directory and ~/.config/PROJECT_NAME are automatically mounted. The
 * PROJECT_NAME define defaults to "ObscureBulldozer", but should be defined
 * accordingly to the current project root directory's name. The FileService
 * can only open files residing in the virtual filesystem. PhysFS needs the path
 * to the executable to determine the project directory. Will work on most
 * platforms automatically, otherwise PHYSFS_init(argv[0]) must be called from
 * main.
 * 
 */
class FileService
{
public:
    /**
     * Constructor.
     */
    FileService();
    /**
     * Destructor.
     */
    ~FileService();

    /**
     * Returns dynamically allocated char* to the file's contents. Used for
     * example to pass glsl shader sources to the compiler.
     *
     * @throw FileNotFoundException When file not found.
     *
     * @param filename The path to the file.
     * @return New'd buffer of the files contents.
     */
    char* fileToBuffer(string filename);

    /**
     * Creates an opengl texture from a PNG image.
     * Loads images from the "assets/images" directory.
     * ".png" is appended to the images' name.
     *
     * @throw FileNotFoundException When image file not found.
     *
     * @param name The name of the image without the path and extension.
     * @return The OpenGL object ID of the created texture.
     */
    GLuint makeTexture(string name);

    /**
     * Creates Entity using all the assets needed. Searches in the path
     * assets/entities for XML-files.
     *
     * @throw EntityCreationError If failed to create Entity.
     * 
     * @param name The name of the entity file without the ".xml".
     * @return A fully initialized Entity object.
     */
    Entity createEntity(string name);

    /**
     * Issues a search in the virtual filesystem and returns the absolute path
     * to the searched path.
     *
     * @throw FileNotFoundException When file not found.
     * @return The absolute path to the searched file.
     */
    string getRealPath(string path);

private:
    fipImage* readImage(string name);

    string dirseparator;

    EntityFactory* m_entity_factory;
};

}

#endif // FILESERVICE_H
