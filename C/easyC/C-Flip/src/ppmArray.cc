#include "ppmArray.h"

using namespace std;

PpmArray::PpmArray()
{
}

int& PpmArray::getSize() 
{
    return size;
}

int &PpmArray::getCapacity()
{
    return capacity;
}

int &PpmArray::getWidth()
{
    return width;
}

int &PpmArray::getHeight()
{
    return height;
}

Pixel*& PpmArray::getPixels()
{
    return pixels;
}

PpmArray::~PpmArray()
{
    if (pixels != NULL)
        delete []pixels;
}

std::istream& operator >>(std::istream &inputStream, PpmArray &other)
{
    string magicNum;
    int maxColVal;

    inputStream >> magicNum;
    inputStream >> other.getWidth() >> other.getHeight()
        >> maxColVal;

    cout << magicNum << "," << maxColVal << endl;
    cout << other.getWidth() << endl;

    inputStream.get(); // skip the trailing white space
    size_t size = other.getWidth() * other.getHeight();
    unsigned char *ptr = new unsigned char[size*3];
    other.getPixels() = new Pixel[size];
    inputStream.read((char*)ptr, size*3);

    Pixel *p = other.getPixels();

    for (size_t i = 0; i < size; i++) {
        p[i].r = ptr[i*3];
        p[i].g = ptr[i*3+1];
        p[i].b = ptr[i*3+2];
    }

    delete []ptr;

    return inputStream;
}

std::ostream& operator <<(std::ostream &outputStream, PpmArray &other)
{
    outputStream << "P6"     << "\n"
        << other.getWidth()       << " "
        << other.getHeight()      << "\n"
        << colourMax   << "\n"
       ;

    size_t size = other.getWidth() * other.getHeight() * 3;
    unsigned char *ptr = new unsigned char[size];
    Pixel *p = other.getPixels();

    for (size_t i = 0; i < size/3; i++) {
        ptr[i*3] = (unsigned char)p[i].r;
        ptr[i*3+1] = (unsigned char)p[i].g;
        ptr[i*3+2] = (unsigned char)p[i].b;
    }
    outputStream.write((char *)ptr, size);

    delete []ptr;

    return outputStream;
}
