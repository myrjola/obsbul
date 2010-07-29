/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include "fileservice.h"

#ifndef LOCATOR_H
#define LOCATOR_H

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

private:
    static FileService* s_file_service;

};

#endif // LOCATOR_H
