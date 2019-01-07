#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

class Vec3D {

    public:
        double x,y,z;
        Vec3D();
        Vec3D(double,double,double);
        //float operator[]();
        Vec3D operator+(Vec3D);
        Vec3D operator*(Vec3D);
        Vec3D operator*(double);
        Vec3D operator/(double scalar);
        double dot(Vec3D);
        Vec3D cross(Vec3D);
        double norm();
        Vec3D normVec();
};

#endif