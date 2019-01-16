#ifndef BMP_H
#define BMP_H

#include "stdio.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include "Matrix.h"
using namespace std;

const int THRESHOLD = 5; // threshold for star recognizion, if grayscale value > THRESHOLD
const int m = 8; // limit for number of ones in 3x3 element
const int L = 36000000; // length from sensor frame
const int D = 12742000; // diameter of earth
const double p_length = 1E-5; // pixel length


class Bmp
{
    private: 
        FILE* f;
        const char* filename;
        int width, height, startindex;
        unsigned char grayscale(unsigned char, unsigned char, unsigned char);

    public:
        Bmp(const char*);
        unsigned char* readRGB();
        int* grayscaleFromRGB(unsigned char*);
        int* digitalize(int*);
        int* erodeBinaryPic(int*);
        int* buildEarthContour(int*, int*);
        Matrix calcCircleMiddlePoint(int*);
        double calcFieldOfViewAndFocalOfLength(int*);
        Vec3D calcEarthVec(Matrix a, double f);
        void writeToTxt(int*, string);
        void writeToBmp(int*, string);
        int getWidth();
        int getHeight();

};

#endif