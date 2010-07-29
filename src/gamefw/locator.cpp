/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/
#include "../common.h"

#include "locator.h"

FileService* Locator::s_file_service;

FileService& Locator::getFileService()
{
    assert(s_file_service != NULL);
    return *s_file_service;
}

void Locator::registerFileService(FileService& service)
{
    s_file_service = &service;
}



