
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

#ifdef _WIN32
typedef unsigned int uint;
#endif

#include "util/logger.h"
using namespace util;

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#endif
