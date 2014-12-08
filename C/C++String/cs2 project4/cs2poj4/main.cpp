/*
 *  main.cpp
 *  Profiler
 *
 *  Created by Jonathan Maletic on 11/8/11.
 *  Copyright 2014 Kent State University. All rights reserved.
 *
 *  Requires main.cpp first, followed by other files.
 *
 *  Modified by:
 *
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "ASTree.hpp"
#include "profile.hpp"


//
// Reads a srcML file into an internal data structure.
// Then prints out the data structure.

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Error: One or more input files are required." << std::endl;
        std::cerr << "       The main must be the first argument followed by any other .cpp files.  For example:" << std::endl;
        std::cerr << "profile main.cpp.xml file1.cpp.xml file2.cpp.xml" << std::endl << std::endl;
        return 1;
    }
    
    srcML                     code;              //The source code to be profiled.
    std::vector<std::string>  files;             //The list of file names 
    std::vector<std::string>  profileNames;      //The list of profile names to be used (no .xml).
    
    for (int i = 1; i < argc; ++i) {
        std::string filename = argv[i];
        files.push_back(filename);
        filename = filename.substr(0, filename.find(".xml"));      //Remove .xml
        std::replace(filename.begin(), filename.end(), '.', '_');  // Change . to _
        profileNames.push_back(filename);
    }
    
    std::ifstream inFile(files[0].c_str());    //Read in the main.
    if (!inFile) {
        std::cerr << "File: " << files[0] << " could not be opened."  << std::endl;
        return 1;
    }
    inFile >> code;                              
    inFile.close();
    	
    code.lineCount(profileNames[0]);           //Count line invocations
	code.funcCount(profileNames[0]);           //Count funciton invocations
    code.mainHeader(profileNames);             //Add in main header info
    code.mainReport(profileNames);             //Add in the report
    
    std::string outFileName = "p-" + files[0];
    outFileName = outFileName.substr(0, outFileName.find(".xml"));  //Remove .xml
    std::ofstream outFile(outFileName.c_str());
    outFile << code << std::endl;
    outFile.close();
    
    for (unsigned i = 1; i < files.size(); ++i) {  //Read in the rest of the files.
        inFile.open(files[i].c_str());
        if (!inFile) {
            std::cerr << "File: " << files[i] << " could not be opened."  << std::endl;
            return 1;
        }        
        inFile >> code;
        inFile.close();
        		        
        code.lineCount(profileNames[i]);           //Count line invocations
		code.funcCount(profileNames[i]);           //Count funciton invocations
        code.fileHeader(profileNames[i]);             //Add in file header info
        
        outFileName = "p-" + files[i];
        outFileName = outFileName.substr(0, outFileName.find(".xml"));  //Remove .xml
        outFile.open(outFileName.c_str());
        outFile << code << std::endl;
        outFile.close();
    }
    
	return 0;
}


