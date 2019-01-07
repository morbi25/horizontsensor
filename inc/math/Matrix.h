#ifndef MATRIX_H
#define MATRIX_H

#include "Vec3D.h"
#include <stdio.h>
#include <stdexcept>

class Matrix {
    private:
        double* matrix;
        int row, column;
    public:
        Matrix(int, int);
        Matrix(Vec3D, Vec3D, Vec3D);
        static Matrix identity();
        ~Matrix();
        void printMatrix();
        Matrix operator*(double scalar);
        Matrix operator*(Matrix);
        Matrix operator+(Matrix);
        double operator() (int, int) const;
        double& operator() (int, int);
        Matrix transpose();
        Matrix inv();
};

#endif