#include <fstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "Picture.h"

using namespace std;

#define MAX_VALUE 255

Picture::Picture(int _height, int _width, Pixel* _image) : height(_height), width(_width), image(_image){}

void Picture::SaveToSingleFile(std::string filename) {
    ofstream out;
    out.open(filename, ios_base::out | ios_base::binary);
    if (!out.is_open()) {
        throw std::runtime_error("Writing error");
    }
    out << "P6" << '\n';
    out << width << ' ' << height << '\n' << MAX_VALUE << '\n';
    char* image_char = (char*)image;
    out.write(image_char, width * height * 3);
    if (out.fail())
        throw std::runtime_error("Writing error");
    out.close();
}

Picture::~Picture() {
    delete[] image;
}