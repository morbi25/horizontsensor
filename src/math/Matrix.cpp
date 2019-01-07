#include "Matrix.h"
using namespace std;

//zero matrix with row rows and column columns
Matrix::Matrix(int row, int column) {
    if(row == 0 || column == 0) throw invalid_argument("The row and column of a matrix has to be greater than zero");
    this->row = row;
    this->column = column;
    this->matrix = new double[row * column];
    for(int i = 0; i < row * column; i++) matrix[i] = 0;
}

//constructor for three vectors -> one vector is a column in the matrix
Matrix::Matrix(Vec3D x, Vec3D y, Vec3D z) {
    this->row = 3;
    this->column = 3;
    this->matrix = new double[9];
    matrix[0] = x.x;
    matrix[1] = y.x;
    matrix[2] = z.x;
    matrix[3] = x.y;
    matrix[4] = y.y;
    matrix[5] = z.y;
    matrix[6] = x.z;
    matrix[7] = y.z;
    matrix[8] = z.z;
}

//create identity matrix 
static Matrix identity(int size) {
    Matrix* result = new Matrix(size, size);
    for(int i = 0; i < size; i++) (*result)(i,i) = 1;
    return *result;
}

//destructor
Matrix::~Matrix() {
  delete[] matrix;
}

//print matrix with three decimal places
void Matrix::printMatrix() {
    for(int i = 0; i < this->row; i++) {
        printf("[   ");
        for(int j = 0; j < this->column; j++) printf("%.3f   ", (*this)(i,j));
        printf("]\n");
    }
}

//multiplication of matrix and scalar
Matrix Matrix::operator*(double scalar) {
    Matrix* result = new Matrix(this->row, this->column);
    for(int i = 0; i < this->row; i++)
        for(int j = 0; j < this->column; j++) (*result)(i,j) = scalar * (*this)(i,j);
    return *result;
}

//multiplication of matrices
Matrix Matrix::operator*(Matrix m) {
    if(this->column != m.row) throw invalid_argument("The number of columns of left matrix has to be equal to the number of rows of the right matrix!");

    Matrix* result = new Matrix(this->row, m.column);

        for(int i = 0; i < result->row; i++)
            for(int j = 0; j < result->column; j++) {
                double sum = 0;
                for(int k = 0; k < m.row; k++) 
                    sum += (*this)(i,k) * m(k,j);
                (*result)(i,j) = sum;
            }

    return *result;
}

//add two matrices of the same size
Matrix Matrix::operator+(Matrix m) {
    if(this->row != m.row || this->column != m.column) throw invalid_argument("The size of the matrices are not identical!");

    Matrix* result = new Matrix(this->row, this->column);

    for(int i = 0; i < result->row; i++)
        for(int j = 0; j < result->column; j++) (*result)(i,j) = (*this)(i,j) + m(i,j);

    return *result;

}

//for reading value from matrix
double Matrix::operator() (int row, int column) const {
    if(row < 0 || row >= this->row || column < 0 || column >= this->column) throw invalid_argument("const Matrix subscript out of bounds!");
    return this->matrix[row * this->column + column];
}

//for changing value in matrix
double& Matrix::operator() (int row, int column) {
    if(row < 0 || row >= this->row || column < 0 || column >= this->column) throw invalid_argument("Matrix subscript out of bounds!");
    return this->matrix[row * this->column + column];
}

//transpose matrix
Matrix Matrix::transpose() {
    Matrix* result = new Matrix(this->column, this->row);
    for(int i = 0; i < result->row; i++)
        for(int j = 0; j < result->column; j++) (*result)(i,j) = (*this)(j,i);
    return *result;
}

//inverse 2x2
Matrix Matrix::inv() {
    if(this->column != 2 && this->row != 2) throw invalid_argument("Matrix has to be 2x2!");

    Matrix* result = new Matrix(2,2);
    (*result)(0,0) = (*this)(1,1);
    (*result)(1,1) = (*this)(0,0);
    (*result)(0,1) = -(*this)(0,1);
    (*result)(1,0) = -(*this)(1,0);

    return (*result) * (1.0 / ((*this)(0,0) * (*this)(1,1) - (*this)(0,1) * (*this)(1,0)));
}
