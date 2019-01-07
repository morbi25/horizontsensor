#include "Vec3D.h"

//Constructors
Vec3D::Vec3D() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vec3D::Vec3D(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

//Operator overload
Vec3D Vec3D::operator+(Vec3D v) {
    return *new Vec3D(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vec3D Vec3D::operator*(Vec3D v) {
    return *new Vec3D(this->x * v.x, this->y * v.y, this->z * v.z);
}

Vec3D Vec3D::operator*(double scalar) {
    return *new Vec3D(scalar * this->x, scalar * this->y, scalar * this->z);
}

Vec3D Vec3D::operator/(double scalar) {
    return *new Vec3D(this->x / scalar, this->y / scalar, this->z / scalar);
}

//Calculate dot product of two 3D vectors
double Vec3D::dot(Vec3D v) {
    return this->x * v.x + this->y * v.y + this->z * v.z;
}

//Calculate cross product of two 3D vectors
Vec3D Vec3D::cross(Vec3D v) {
    return *new Vec3D(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y *v.x);
}

//Calculate norm of 3D vector
double Vec3D::norm() {
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

//Normalize the given 3D vector
Vec3D Vec3D::normVec() {
    return (*this) * (1.0 / this->norm());
}