/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/

#ifndef FILESERVICE_H
#define FILESERVICE_H

#include "../common.h"

#include <GL/gl.h>

class fipImage;

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
    char* fileToBuffer ( string filename );

    /**
     * Creates an opengl texture and returns it's ID.
     * Loads images from the "assets/images" directory.
     *
     * @throw FileNotFoundException When image file not found.
     *
     * @param name The name of the image without the path and extension.
     * @return The OpenGL object ID of the created texture.
     */
    GLuint makeTexture ( string name );

private:
    const fipImage& readImage ( string name );
};

#endif // FILESERVICE_H
