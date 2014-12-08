////////////////////////////////////////////////////////////////////
// File:         simple.cpp
// Creation:     4/2013
// Programmer:   Dr. J. Maletic
//
// Description:  Simple program for testing profiler
//
//
#include <iostream>



#include"profile.hpp"
profile simple_cpp("simple.cpp");
int search(int tbl[], int n, int key) {simple_cpp.count(__LINE__,"search");

    int result = -1;simple_cpp.count(__LINE__);

    for (int i = 0; i < n; ++i) {
        if (key == tbl[i]) {
            result = i;simple_cpp.count(__LINE__);

        }
    }
    return result;
}



int main() {simple_cpp.count(__LINE__,"main");


    int lst[5] = {2, 4, 6, 8, 10};simple_cpp.count(__LINE__);

    std::cout << search(lst, 5, 6);simple_cpp.count(__LINE__);

    std::cout << std::endl;simple_cpp.count(__LINE__);

    
    std::cout << "Done";simple_cpp.count(__LINE__);

    std::cout << std::endl;simple_cpp.count(__LINE__);

    
std::cout << simple_cpp << std::endl;
return 0;
}

