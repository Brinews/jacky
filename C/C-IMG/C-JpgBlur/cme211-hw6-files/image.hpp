#ifndef _IMAGE_H
#define _IMAGE_H

#include <boost/multi_array.hpp>
#include <string>

using namespace std;

class Image {
    private:
        boost::multi_array<unsigned char, 2> img;
        boost::multi_array<unsigned char, 2> out;
        boost::multi_array<unsigned char, 2> out1;
        int width;
        int height;
        string filename;
    public:
        Image(string name);
        ~Image();

        void Save(string savename);

        void BoxBlur(int kernelsize);
        unsigned int Sharpness();
};

void Convolution(boost::multi_array<unsigned char,2>& input,
        boost::multi_array<unsigned char,2>& output,
        boost::multi_array<float,2>& kernel);

#endif
