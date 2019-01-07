#include "Quaternion.h"

Quaternion::Quaternion(double q0, double q1, double q2, double q3) {
    this->q0 = q0;
    this->q1 = q1;
    this->q2 = q2;
    this->q3 = q3;
}


void Quaternion::printQuat() {
    printf("[%f, %f, %f, %f]", this->q0, this->q1, this->q2, this->q3);
}