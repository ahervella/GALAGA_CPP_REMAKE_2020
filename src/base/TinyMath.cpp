//
// Created by Alejandro Hervella on 2/6/20.
//


// Support Code written by Michael D. Shah
// Updated by Seth Cooper
// Last Updated: Spring 2020
// Please do not redistribute without asking permission.
#include <TinyMath.hpp>


// Vector3D performs vector operations with 3-dimensions
// The purpose of this class is primarily for 3D graphics
// applications.

// Default constructor
// 'why default?' https://stackoverflow.com/questions/20828907/the-new-keyword-default-in-c11
//Vector3D::Vector3D() = default;

// The "Real" constructor we want to use.
// This initializes the values x,y,z
Vector3D::Vector3D(float a, float b, float c) {
    // TODO:
    x = a;
    y = b;
    z = c;
}

//2D vector (x and y set, z = 0)
Vector3D::Vector3D(float a, float b) {
	x = a;
	y = b;
	z = 0;
}

// Index operator, allowing us to access the individual
// x,y,z components of our vector.
float &Vector3D::operator[](int i) {
    // TODO: Discuss with your partner why this works.
    //       There is no code to change here.
    return ((&x)[i]);
}

// Index operator, allowing us to access the individual
// x,y,z components of our vector.
const float &Vector3D::operator[](int i) const {
    // TODO: Discuss with your partner why this works.
    //       There is no code to change here.
    return ((&x)[i]);
}

// Multiplication Operator
// Multiply vector by a uniform-scalar.
//Test proper modification of this vector
Vector3D &Vector3D::operator*=(float s) {
    x *= s;
    y *= s;
    z *= s;
    return (*this);
}

// Division Operator
//Test proper modification of this vector and proper exception throwing when dividing by zero
Vector3D &Vector3D::operator/=(float s) {
    // TODO:
    if (s == 0) {
        throw "Divide by zero!";
    }
    x /= s;
    y /= s;
    z /= s;
    return (*this);
}

// Addition operator
//Ensure proper modification of this vector, no modification of vector v
Vector3D &Vector3D::operator+=(const Vector3D &v) {
    // TODO:
    x += v[0];
    y += v[1];
    z += v[2];
    return (*this);
}

// Subtraction operator
//Ensure proper modification of this vector, no modification of vector v
Vector3D &Vector3D::operator-=(const Vector3D &v) {
    // TODO:
    x -= v[0];
    y -= v[1];
    z -= v[2];
    return (*this);
}


// Compute the dot product of a Vector3D
//Test correct computation, maybe test against Mag(A)Mag(B)cos(angle)
inline float Dot(const Vector3D &a, const Vector3D &b) {
    // TODO:
    float xProduct = a[0] * b[0];
    float yProduct = a[1] * b[1];
    float zProduct = a[2] * b[2];
    float dotProduct = xProduct + yProduct + zProduct;
    return dotProduct;
}

// Multiplication of a vector by a scalar values
//Test proper modification of vec, and make sure v is not modified (const should enforce this)
/*
inline Vector3D operator*(const Vector3D &v, float s) {
    // TODO:
    Vector3D vec;
    vec[0] = v[0] * s;
    vec[1] = v[1] * s;
    vec[2] = v[2] * s;
    return vec;
}
*/

// Division of a vector by a scalar value.
//Test that divide by zero exception is thrown, test proper division, test no modification of v.
inline Vector3D operator/(const Vector3D &v, float s) {
    // TODO:
    //We don't want any divide by zeros...
    if (s == 0) {
        throw "Divide by zero!";
    }
    Vector3D vec;
    vec[0] = v[0] / s;
    vec[1] = v[1] / s;
    vec[2] = v[2] / s;
    return vec;
}

// Negation of a vector
// Use Case: Sometimes it is handy to apply a force in an opposite direction
//Test proper negation, test no modification of v
inline Vector3D operator-(const Vector3D &v) {
    // TODO:
    Vector3D vec;
    vec[0] = -v[0];
    vec[1] = -v[1];
    vec[2] = -v[2];
    return vec;
}


// Return the magnitude of a vector
//Test proper calculation, test no modification of v
inline float Magnitude(const Vector3D &v) {
    // TODO:
    float xSquared = pow(v[0], 2);
    float ySquared = pow(v[1], 2);
    float zSquared = pow(v[2], 2);
    float sumOfSquares = xSquared + ySquared + zSquared;
    float magnitude = sqrt(sumOfSquares);
    return magnitude;
}

// Add two vectors together
//Test proper calculation, no modification of a or b
inline Vector3D operator+(const Vector3D &a, const Vector3D &b) {
    // TODO:
    Vector3D vec;
    vec[0] = a[0] + b[0];
    vec[1] = a[1] + b[1];
    vec[2] = a[2] + b[2];
    return vec;
}

// Subtract two vectors
//Test proper calculation, no modification of a or b
inline Vector3D operator-(const Vector3D &a, const Vector3D &b) {
    // TODO:
    Vector3D vec;
    vec[0] = a[0] - b[0];
    vec[1] = a[1] - b[1];
    vec[2] = a[2] - b[2];
    return vec;
}

// Set a vectors magnitude to 1
// Note: This is NOT generating a normal vector
//Normalized (Unit) vector = v / Magnitude(v)
//Test proper result (do each step individually to calculate), no modification of v
inline Vector3D Normalize(const Vector3D &v) {
    // TODO:
    Vector3D vec;
    vec = v / Magnitude(v);
    return vec;
}

// Vector Projection
//Projection of a onto b is (Dot(a, b)/Mag(b)^2) * b
//Test proper result (do each step individually to calculate), no modification of a or b
inline Vector3D Project(const Vector3D &a, const Vector3D &b) {
    // TODO:
    Vector3D vec;
    float scalarProject = Dot(a, b) / pow(Magnitude(b), 2);
    vec = b * scalarProject;
    return vec;
}


//gets the angle between two 3D vectors (in radians),
//assume the origin is the reference point.
inline float getRadAngle(Vector3D a, Vector3D b) {

    float dotProd = Dot(a, b);

    float magAB = Magnitude(a) * Magnitude(b);

    return acos(dotProd / magAB);

}

//gets the mid point of two vectors (or 3D points)
inline Vector3D MidPoint(Vector3D a, Vector3D b) {
    Vector3D midPointV = (a + b) / 2.0f;

    return midPointV;
}

//calculates the distance between two vectors (or 3D points).
inline float Distance(Vector3D a, Vector3D b) {
    return Magnitude(a - b);
}

// a x b (read: 'a crossed b')
// Produces a new vector perpendicular to a and b.
// (So long as a and b are not parallel which returns zero vector)
// CrossProduct = (aybz - azby, azbx - axbz, axby - aybx)
// Test proper calculation
inline Vector3D CrossProduct(const Vector3D &a, const Vector3D &b) {
    // TODO:
    Vector3D vec;
    vec[0] = (a[1] * b[2]) - (a[2] * b[1]);
    vec[1] = (a[2] * b[0]) - (a[0] * b[2]);
    vec[2] = (a[0] * b[1]) - (a[1] * b[0]);
    return vec;
}


//Matrix3D::Matrix3D() = default;

// TODO: Row or column major order you decide! Let's go row major!
// Matrix constructor with 9 scalar values.
Matrix3D::Matrix3D(float n00, float n01, float n02,
                   float n10, float n11, float n12,
                   float n20, float n21, float n22) {

    n[0][0] = n00;
    n[0][1] = n01;
    n[0][2] = n02;
    n[1][0] = n10;
    n[1][1] = n11;
    n[1][2] = n12;
    n[2][0] = n20;
    n[2][1] = n21;
    n[2][2] = n22;
}

// Matrix constructor from three vectors.
Matrix3D::Matrix3D(const Vector3D &a, const Vector3D &b, const Vector3D &c) {
    n[0][0] = a.x;
    n[0][1] = a.y;
    n[0][2] = a.z;
    n[1][0] = b.x;
    n[1][1] = b.y;
    n[1][2] = b.z;
    n[2][0] = c.x;
    n[2][1] = c.y;
    n[2][2] = c.z;
}

// Index operator with two dimensions
// Example: M(1,1) returns row 1 and column 1 of matrix M.
float &Matrix3D::operator()(int i, int j) {
    return (n[i][j]);
}

// Index operator with two dimensions
// Example: M(1,1) returns row 1 and column 1 of matrix M.
const float &Matrix3D::operator()(int i, int j) const {
    return (n[i][j]);
}

// Return a row from a matrix as a vector.
Vector3D &Matrix3D::operator[](int j) {
    return (*reinterpret_cast<Vector3D *>(n[j]));
}

// Return a row from a matrix as a vector.
const Vector3D &Matrix3D::operator[](int j) const {
    return (*reinterpret_cast<const Vector3D *>(n[j]));
}

//Just uses [] operator, making this a separate function for readability's sake
//Makes more sense if a getCol is implemented alongside this
Vector3D &Matrix3D::getRow(int j) {
    return this->operator[](j);
}

//Const version
const Vector3D &Matrix3D::getRow(int j) const {
    return this->operator[](j);
}

//Cannot return a reference to a vector since the column values are not stored contiguously in memory (index access would not work properly)
//Ensure this returns the proper column values
Vector3D Matrix3D::getCol(int j) {
    Vector3D vec;
    vec[0] = this->operator()(0, j);
    vec[1] = this->operator()(1, j);
    vec[2] = this->operator()(2, j);
    return vec;
}

//Const version
const Vector3D Matrix3D::getCol(int j) const {
    Vector3D vec;
    vec[0] = this->operator()(0, j);
    vec[1] = this->operator()(1, j);
    vec[2] = this->operator()(2, j);
    return vec;
}

Matrix3D Matrix3D::transpose() {

    Vector3D row0 = Vector3D(this->getCol(0)[0], this->getCol(0)[1], this->getCol(0)[2]);
    Vector3D row1 = Vector3D(this->getCol(1)[0], this->getCol(1)[1], this->getCol(1)[2]);
    Vector3D row2 = Vector3D(this->getCol(2)[0], this->getCol(2)[1], this->getCol(2)[2]);

    return Matrix3D(row0, row1, row2);

}


// Matrix multiply by a vector
// Resulting matrix is a 1x3, so a Vector
// Basically a multiplication of a 1x3 matrix and a 3x3 matrix
// Reminder that element i,j of result is Dot(A[i], B[j]), so result will just be a 1x3 matrix of Dot(v, M[0..2])
// Verify the math (so maybe compare to manual dot product)
Vector3D operator*(const Matrix3D &M, const Vector3D &v) {
    // TODO:
    Vector3D vec;
    vec[0] = Dot(v, M.getCol(0));
    vec[1] = Dot(v, M.getCol(1));
    vec[2] = Dot(v, M.getCol(2));
    return vec;
}

// Matrix Multiplication
//Each element i,j in the product matrix is the dot product of row i in A and column j in B
// Test for correctness
Matrix3D operator*(const Matrix3D &A, const Matrix3D &B) {
    // TODO:
    Matrix3D mat3D;

    //Vectors to represent the three resulting rows
    //Unfortunately, the matrix * vector operation does not follow the actual order of the matrices for the multiplication (order matters)
    //So B * A.getRow(j) actually means A.getRow(j) * B (a 1x3 * 3x3 --> 1x3 result, which is a Vector3D)
    const Vector3D row0 = B * A.getRow(0);
    const Vector3D row1 = B * A.getRow(1);
    const Vector3D row2 = B * A.getRow(2);

    mat3D = Matrix3D(row0, row1, row2);

    return mat3D;
}


//degrees to radians converter
float degToRad(float deg) {

    return deg * (PI / 180.0);
}

//return reflected angle in radians
float reflectRadAngle(float angle, bool aboutX) {

    if (aboutX) {
        return angle * -1;
    }

    return angle * -1 + PI;

}
