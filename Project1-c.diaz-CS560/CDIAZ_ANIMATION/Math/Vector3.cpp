#include "Vector3.h"
#include <cmath>
#include <stdexcept>

Vector3::Vector3() : x(0), y(0), z(0)
{
	Zero();
}


Vector3::~Vector3()
{
}


Vector3::Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz)
{
	
}


Vector3::Vector3(const Vector3& vec) : x(vec.x), y(vec.y), z(vec.z)
{

}
Vector3::Vector3( Vector2& vec) :x(vec.GetX()), y(vec.GetY()), z(0)
{

}

Vector3::Vector3( Vector2& vec, float z) : x(vec.GetX()), y(vec.GetY()), z(z)
{

}

void Vector3::Zero()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

float Vector3::GetX() const
{
	return x;
}

void Vector3::SetX(float nx)
{
	x = nx;
}

float Vector3::GetY() const
{
	return y;
}

void Vector3::SetY(float ny)
{
	y = ny;
}

float Vector3::GetZ() const
{
	return z;
}

void Vector3::SetZ(float nz)
{
	z = nz;
}

Vector3& Vector3::operator=( Vector3& vec)
{
	
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
	return *this;
}

Vector3 Vector3::operator+( Vector3& vec)
{
	Vector3 result;
	result.x = this->x + vec.GetX();
	result.y = this->y + vec.GetY();
	result.z = this->z + vec.GetZ();
	return result;
}
Vector3 Vector3::operator-( Vector3& vec)
{
	Vector3 result;
	result.x = this->x - vec.GetX();
	result.y = this->y - vec.GetY();
	result.z = this->z - vec.GetZ();
	return result;
}
Vector3& Vector3::operator*( float c)
{

	this->x = this->x * c;
	this->y = this->y *c;
	this->z = this->z * c;
	return *this;
}



float Vector3::operator[](int index)
{
	return data[index];
}

Vector3 operator*(float c, Vector3& vec)
{
	Vector3 result;
	result.x = vec.x * c;
	result.y = vec.y * c;
	result.z = vec.z * c;
	return result;
}

Vector3 operator*( Vector3& a,  Vector3& b)
{
	Vector3 result;
	for (int i = 0; i < 3; i++)
	{
		result.x = a.x * b.x;
		result.y = a.y * b.y;
		result.z = a.z * b.z;
	}

	return result;
}

Vector3 operator/(Vector3& a, Vector3& b)
{
	Vector3 result;
	for (int i = 0; i < 3; i++)
	{
		result.x = a.x / b.x;
		result.y = a.y / b.y;
		result.z = a.z / b.z;
	}

	return result;
}

Vector3 Vector3::operator += (Vector3& vec)
{
	this->x += vec.x;
	this->y += vec.y;
	this->z += vec.z;

	return *this;
}

std::ostream& operator<<(std::ostream& os, Vector3& vec)
{
	return std::cout << vec.x << " " << vec.y << " " << vec.z << " ";
}