#pragma once
#include "Vector4.h"
#include <array>

using namespace std;

class Matrix4
{
public:
	
	Matrix4();
	Matrix4(float x);
	Matrix4(Vector4& v1, Vector4& v2, Vector4& v3, Vector4& v4);
	~Matrix4();	
	
	Vector4& operator[](int index);
	Matrix4 operator+(Matrix4& m2);
	Matrix4 operator-(Matrix4& m2);
	Matrix4 operator*(float scalar);
	
	Matrix4 transpose( Matrix4& m);

	Matrix4& operator*=(float scalar);

	
	//Matrix4& operator=(aiMatrix4x4 mat);
	
	Vector4 data[4];

	void Zero();

private:
	
};
Matrix4 operator*(Matrix4& a, Matrix4& b);
Vector4 operator*(Matrix4& a, Vector4& v);
Vector4 operator*(Vector4& v, Matrix4& a);
std::ostream& operator<<(std::ostream& os, Matrix4& dt);