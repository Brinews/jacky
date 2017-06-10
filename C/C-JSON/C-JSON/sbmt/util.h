#ifndef _UTIL_H
#define _UTIL_H

#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "node.h"

std::string printValueString( Int value );
std::string printValueString( UInt value );
std::string printValueString( double value );
std::string printValueString( bool value );
std::string printValueString( const char *value );


#endif
