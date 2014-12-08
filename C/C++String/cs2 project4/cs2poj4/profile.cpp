/*
 *  profile.cpp
 *
 *  Created by Jonathan Maletic on 3/29/2012.
 *  Copyright 2014 Kent State University. All rights reserved.
 *
 *  Modified by:
 *
 */

#include "profile.hpp"

////////////////////////////////////////////////////////////////////////
// Prints out the profile.
//
// TODO: Very simple output, need columns with nice headings.
// 
std::ostream& operator<< (std::ostream& out, const profile& prof) {	
	//need sort
	out << "File: " << prof.fileName << std::endl;
    for(std::map<std::string, int>::const_iterator i = prof.item.begin(); i != prof.item.end(); ++i) {
        out << i->first << " " << i->second << std::endl;        
    }
    out << std::endl;

    return out;
}



////////////////////////////////////////////////////////// 
// PRE:  number >= 0
// POST: Returns a text version of a positive integer long
std::string intToString(int number) {
    assert(number >= 0);
    std::string result;
    
    if (number == 0) return "0";
    while (number > 0) {
        result = char(int('0') + (number % 10)) + result;
        number = number / 10;
    }  
    return result;
}
