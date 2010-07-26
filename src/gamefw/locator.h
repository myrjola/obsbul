/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include "fileservice.h"

#ifndef LOCATOR_H
#define LOCATOR_H

// Class using the service locator pattern. Used for example to get file system
// services or audio.
class Locator
{
    public:
        // Returns the file system service.
        static const FileService& getFileService();

        // Binds the given service as the located file service.
        static void RegisterFileService(FileService& service);

    private:
        static FileService file_service;
                
};

#endif // LOCATOR_H
