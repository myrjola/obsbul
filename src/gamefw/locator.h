#ifndef LOCATOR_H
#define LOCATOR_H

#include "fileservice.h"
#include "shaderfactory.h"

namespace gamefw {

/**
 * @brief Used to locate engine "services".
 * Class using the service locator pattern. Used for example to get file system
 * services or audio.
 */
class Locator
{
public:
    /**
     * @return The file system service.
     */
    static FileService& getFileService();

    /**
     * Binds a service as the service located by getFileService().
     *
     * @param service The service to be bound.
     */
    static void registerFileService(FileService& service);

    /**
     * @return The shader factory service.
     */
    static ShaderFactory& getShaderFactory();

    /**
     * Binds a service as the service located by getShaderFactory().
     *
     * @param service The service to be bound.
     */
    static void registerShaderFactory(gamefw::ShaderFactory& service);

private:
    static FileService* s_file_service;
    static ShaderFactory* s_shader_factory;
};

}

#endif // LOCATOR_H
