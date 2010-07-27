/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include "../common.h"

#include "locator.h"

FileService* Locator::file_service;

FileService& Locator::getFileService()
{
    assert(file_service != NULL);
    return *file_service;
}

void Locator::RegisterFileService(FileService& service)
{
    file_service = &service;
}



