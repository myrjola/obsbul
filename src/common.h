/*
Copyright (c) 2010 Martin Yrjölä <martin.yrjola@gmail.com>

*/

#ifndef COMMON_H
#define COMMON_H

#include <cassert>
#include <iostream>
#include <cstring>
#include <vector>
#include <exception>

using namespace std;

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#include <glog/logging.h>

#endif