#include "Bmp.h"
#include "Matrix.h"

Bmp file("Erde.bmp");

int width = file.getWidth(), heigth = file.getHeight(); //Width and heigth of the picture

int main() {
    //Read rgb picture
    unsigned char* rgb_picture = file.readRGB();
    //convert rgb to grayscale picture
    int* grayscale_picture = file.grayscaleFromRGB(rgb_picture);
    //binarization of grayscale picture and write it to bmp
    int* dig = file.digitalize(grayscale_picture);
    printf("\nAufgabe 1 b) (1):\n");
    file.writeToBmp(dig, "dig.bmp");
    //Calculate field of view and focal of length, f is returned because it will be used by other calculations
    printf("\nAufgabe 1 a):\n");
    double f = file.calcFieldOfViewAndFocalOfLength(dig);
    //Calculate erode picture and write it to bmp
    int* erod = file.erodeBinaryPic(dig);
    printf("\nAufgabe 1 b) (2):\n");
    file.writeToBmp(erod, "erod.bmp");
    //Calculate subtraction picture (of binary and erode picture) and write it to bmp
    int* diff = file.buildEarthContour(dig, erod);
    printf("\nAufgabe 1 b) (3):\n");
    file.writeToBmp(diff, "diff.bmp");
    //Calculate middle point of difference picture
    Matrix a = file.calcCircleMiddlePoint(diff);
    printf("\nAufgabe 1 b) (4):\nm = [%f, %f]\n", a(0,0), a(1,0));
    //Calculate normed Earth Vector
    Vec3D normedEarthVec = file.calcEarthVec(a, f);
    printf("\nAufgabe 1 b) (5):\nv_Erde = [%f,%f,%f]\n", normedEarthVec.x, normedEarthVec.y, normedEarthVec.z); 
}