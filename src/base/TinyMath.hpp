//
// Created by Alejandro Hervella on 2/6/20.
//

#ifndef PROJ1_BREAKOUT_MIKEY_BREAKOUT_TINYMATH_HPP
#define PROJ1_BREAKOUT_MIKEY_BREAKOUT_TINYMATH_HPP


// Support Code written by Michael D. Shah
// Updated by Seth Cooper
// Last Updated: Spring 2020
// Please do not redistribute without asking permission.
#include <math.h>

#define PI 3.14159

// Forward references of each of the structs
struct Vector3D;
struct Matrix3D;

// Vector3D performs vector operations with 3-dimensions
// The purpose of this class is primarily for 3D graphics
// applications.
struct Vector3D{
    // Note: x,y,z are a convention
    // x,y,z could be position, but also any 3-component value.
    float x,y,z;

    // Default constructor
    // 'why default?' https://stackoverflow.com/questions/20828907/the-new-keyword-default-in-c11
    Vector3D() = default;

    // The "Real" constructor we want to use.
    // This initializes the values x,y,z
    Vector3D(float a, float b, float c);

    //For a 2D vector (sets x and y, z = 0)
    Vector3D(float a, float b);

    // Index operator, allowing us to access the individual
    // x,y,z components of our vector.
    float& operator[](int i);

    // Index operator, allowing us to access the individual
    // x,y,z components of our vector.
    const float& operator[](int i) const;

    // Multiplication Operator
    // Multiply vector by a uniform-scalar.
    //Test proper modification of this vector
    Vector3D& operator *=(float s);

    // Division Operator
    //Test proper modification of this vector and proper exception throwing when dividing by zero
    Vector3D& operator /=(float s);

    // Addition operator
    //Ensure proper modification of this vector, no modification of vector v
    Vector3D& operator +=(const Vector3D& v);

    // Subtraction operator
    //Ensure proper modification of this vector, no modification of vector v
    Vector3D& operator -=(const Vector3D& v);

};

// Compute the dot product of a Vector3D
//Test correct computation, maybe test against Mag(A)Mag(B)cos(angle)
inline float Dot(const Vector3D& a, const Vector3D& b);

// Multiplication of a vector by a scalar values
//Test proper modification of vec, and make sure v is not modified (const should enforce this)
//inline Vector3D operator *(const Vector3D& v, float s);
inline Vector3D operator*(const Vector3D &v, float s) {
    // TODO:
    Vector3D vec;
    vec[0] = v[0] * s;
    vec[1] = v[1] * s;
    vec[2] = v[2] * s;
    return vec;
}

// Division of a vector by a scalar value.
//Test that divide by zero exception is thrown, test proper division, test no modification of v.
inline Vector3D operator /(const Vector3D& v, float s);

// Negation of a vector
// Use Case: Sometimes it is handy to apply a force in an opposite direction
//Test proper negation, test no modification of v
inline Vector3D operator -(const Vector3D& v);


// Return the magnitude of a vector
//Test proper calculation, test no modification of v
inline float Magnitude(const Vector3D& v);

// Add two vectors together
//Test proper calculation, no modification of a or b
inline Vector3D operator +(const Vector3D& a, const Vector3D& b);

// Subtract two vectors
//Test proper calculation, no modification of a or b
inline Vector3D operator -(const Vector3D& a, const Vector3D& b);

// Set a vectors magnitude to 1
// Note: This is NOT generating a normal vector
//Normalized (Unit) vector = v / Magnitude(v)
//Test proper result (do each step individually to calculate), no modification of v
inline Vector3D Normalize(const Vector3D& v);

// Vector Projection
//Projection of a onto b is (Dot(a, b)/Mag(b)^2) * b
//Test proper result (do each step individually to calculate), no modification of a or b
inline Vector3D Project(const Vector3D& a, const Vector3D& b);


//gets the angle between two 3D vectors (in radians),
//assume the origin is the reference point.
inline float getRadAngle(Vector3D a, Vector3D b);

//gets the mid point of two vectors (or 3D points)
inline Vector3D MidPoint(Vector3D a, Vector3D b);

//calculates the distance between two vectors (or 3D points).
inline float Distance(Vector3D a, Vector3D b);

// a x b (read: 'a crossed b')
// Produces a new vector perpendicular to a and b.
// (So long as a and b are not parallel which returns zero vector)
// CrossProduct = (aybz - azby, azbx - axbz, axby - aybx)
// Test proper calculation
inline Vector3D CrossProduct(const Vector3D& a, const Vector3D& b);


// Matrix 3D represents 3x3 matrices in Math
struct Matrix3D{
private:
    float n[3][3];  // Store each value of the matrix

public:
    Matrix3D() = default;

    // TODO: Row or column major order you decide! Let's go row major!
    // Matrix constructor with 9 scalar values.
    Matrix3D( float n00, float n01, float n02,
              float n10, float n11, float n12,
              float n20, float n21, float n22);

    // Matrix constructor from three vectors.
    Matrix3D(const Vector3D& a, const Vector3D& b, const Vector3D& c);

    // Index operator with two dimensions
    // Example: M(1,1) returns row 1 and column 1 of matrix M.
    float& operator ()(int i, int j);

    // Index operator with two dimensions
    // Example: M(1,1) returns row 1 and column 1 of matrix M.
    const float& operator ()(int i, int j) const;

    // Return a row from a matrix as a vector.
    Vector3D& operator [](int j);

    // Return a row from a matrix as a vector.
    const Vector3D& operator [](int j) const;

    //Just uses [] operator, making this a separate function for readability's sake
    //Makes more sense if a getCol is implemented alongside this
    Vector3D& getRow(int j);

    //Const version
    const Vector3D& getRow(int j) const;

    //Cannot return a reference to a vector since the column values are not stored contiguously in memory (index access would not work properly)
    //Ensure this returns the proper column values
    Vector3D getCol(int j);

    //Const version
    const Vector3D getCol(int j) const;

    Matrix3D transpose();

};


// Matrix multiply by a vector
// Resulting matrix is a 1x3, so a Vector
// Basically a multiplication of a 1x3 matrix and a 3x3 matrix
// Reminder that element i,j of result is Dot(A[i], B[j]), so result will just be a 1x3 matrix of Dot(v, M[0..2])
// Verify the math (so maybe compare to manual dot product)
Vector3D operator *(const Matrix3D& M, const Vector3D& v);

// Matrix Multiplication
//Each element i,j in the product matrix is the dot product of row i in A and column j in B
// Test for correctness
Matrix3D operator *(const Matrix3D& A, const Matrix3D& B);


//degrees to radians converter
float degToRad(float deg);

//return reflected angle in radians
float reflectRadAngle(float angle, bool aboutX);





#endif //PROJ1_BREAKOUT_MIKEY_BREAKOUT_TINYMATH_HPP
