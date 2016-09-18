#include "Matrix4.h"


Matrix4::Matrix4() 
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (i == j)
				data[i][j] = 1.0f ;
			else
				data[i][j] = 0.0f;
}

Matrix4::Matrix4(float x) :data()
{
	data[0] = Vector4(x, 0.0f, 0.0f, 0.0f);
	data[1] = Vector4(0.0f, x, 0.0f, 0.0f);
	data[2] = Vector4(0.0f, 0.0f, x, 0.0f);
	data[3] = Vector4(0.0f, 0.0f, 0.0f, x);
}

Matrix4::Matrix4(Vector4& v1, Vector4& v2, Vector4& v3, Vector4& v4)
{
	data[0] = v1;
	data[1] = v2;
	data[2] = v3;
	data[3] = v4;
}

Matrix4::~Matrix4()
{
}

Vector4& Matrix4::operator[](int index)
{
	return data[index];
}

void Matrix4::Zero()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
				data[i][j] = 0.0f;
}


Matrix4 Matrix4::operator+(Matrix4& m2)
{
	Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		result[i] = data[i] + m2.data[i];
	}

	return result;
}

Matrix4 Matrix4::operator-(Matrix4& m2)
{
	Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		result[i] = data[i] - m2.data[i];
	}

	return result;
}

Matrix4 Matrix4::operator*(float scalar)
{
	Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		result[i] = data[i] * scalar;
	}

	return result;
}

Matrix4 operator*(Matrix4& a, Matrix4& b)
{
	
	Matrix4 result;
	Vector4 value = a[0] * b[0][0];
	Vector4 value1 = a[1] * b[0][1];
	Vector4 value2 = a[2] * b[0][2];
	Vector4 value3 = a[3] * b[0][3];
	result[0] = value + value1 + value2 + value3;
	
	
	

	 value = a[0] * b[1][0];
	 value1 = a[1] * b[1][1];
	 value2 = a[2] * b[1][2];
	 value3 = a[3] * b[1][3];
	 result[1] = value + value1 + value2 + value3;
	
	result[2] = a[0] * b[2][0] + a[1] * b[2][1] + a[2] * b[2][2] + a[3] * b[2][3];
	
	result[3] = a[0] * b[3][0] + a[1] * b[3][1] + a[2] * b[3][2] + a[3] * b[3][3];
	
	
	

	return result;


}

Vector4 operator*(Matrix4& a, Vector4& v)
{
	 Vector4 mul0 = a[0] * v[0];
	 Vector4 mul1 = a[1] * v[1];
	 Vector4 mul2 = a[2] * v[2];
	 Vector4 mul3 = a[3] * v[3];

	 Vector4 add0 = mul0 + mul1;
	 Vector4 add1 = mul2 + mul3;

	return add0 + add1;
}

Matrix4 Matrix4::transpose( Matrix4& m)
{
	Matrix4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			result[i][j] = m[j][i];
	}
	return result;
}

std::ostream& operator<<(std::ostream& os, Matrix4& m)
{
	std::cout << "Matrix 4:" << endl;
	for (int i = 0; i < 4; i++)
	{
		
		std::cout<<	 m[i];
		
	}

	return cout << endl;
}

Matrix4& Matrix4::operator*=(float scalar)
{
	for (int i = 0; i < 4; i++)
	{
		this->data[i] = this->data[i]  * scalar;
	}

	return *this;
}