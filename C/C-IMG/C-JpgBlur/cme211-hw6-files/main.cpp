/*
 * Date: 2016-12-01
 * Purpose: Jpeg image blur and sharpness
 */

#include "image.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char **argv)
{
	int dim = 28;
    Image img("stanford.jpg");

    printf("Original image:%4d\n", 255);

    for (int i = 3; i < dim; i+=4) {

        img.BoxBlur(i);

        char name[20];
        sprintf(name, "BoxBlur%02d.jpg", i);
        string filename(name);
        img.Save(filename);

        printf("BoxBlur(%2d):%7d\n", i, img.Sharpness());
    }

    return 0;
}
