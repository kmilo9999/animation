#include "Vector4.h"
#include <cmath>
#include <stdexcept>

Vector4::Vector4() :x(0), y(0), z(0), w(0)
{
}


Vector4::~Vector4()
{
}

Vector4::Vector4(float nx, float ny, float nz, float nw) : x(nx), y(ny), z(nz), w(nw)
{
	
}

Vector4::Vector4(const Vector4& vec) : x(vec.x), y(vec.y) , z(vec.z), w(vec.w)
{

}



Vector4::Vector4( Vector3& vec) :x(vec.GetX()), y(vec.GetY()), z(vec.GetZ()), w(0)
{
	
}

Vector4::Vector4( Vector3& vec, float w): x(vec.GetX()), y(vec.GetY()), z(vec.GetZ()), w(w)
{

}

Vector4::Vector4( Vector2& vec) : x(vec.GetX()), y(vec.GetY()), z(0), w(0)
{

}

Vector4::Vector4( Vector2& vec, float z, float w): x(vec.GetX()), y(vec.GetY()), z(z), w(w)
{

}

void Vector4::Zero()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

float Vector4::GetX() const
{
	return x;
}

void Vector4::SetX(float nx)
{
	x = nx;
}

float Vector4::GetY() const
{
	return y;
}

void Vector4::SetY(float ny)
{
	y = ny;
}

float Vector4::GetZ() const
{
	return z;
}
void Vector4::SetZ(float nz)
{
	z = nz;
}

float Vector4::GetW() const
{
	return w;
}
void Vector4::SetW(float nw)
{
	w = nw;
}

Vector4& Vector4::operator=( Vector4 vec)
{
	x = vec.GetX();
	y = vec.GetY();
	z = vec.GetZ();
	w = vec.GetW();
	return *this;
}

Vector4 Vector4::operator+( Vector4& vec)
{
	Vector4 result;
	result.x = this->x + vec.x;
	result.y = this->y + vec.y;
	result.z = this->z + vec.z;
	result.w = this->w + vec.w;
	return result;
}

Vector4 Vector4::operator-( Vector4& vec)
{
	Vector4 result;
	result.x = this->x - vec.x;
	result.y = this->y - vec.y;
	result.z = this->z - vec.z;
	result.w = this->w - vec.w;
	return result;
}

Vector4 Vector4::operator*( float c)
{
	Vector4 result;
	result.x = this->x * c;
	result.y = this->y * c;
	result.z = this->z * c;
	result.w = this->w * c;
	return result;
}


float& Vector4::operator[](int index)
{
	return data[index];
}

Vector4 operator*(Vector4& a, Vector4& b)
{
	Vector4 result;
	for (int i = 0; i < 4; i++)
		result[i] = a[i] * b[i];
	return result;
}

std::ostream& operator<<(std::ostream& os, Vector4& v)
{
	return std::cout << "( " << v.x << "," << v.y << "," << v.z << "," << v.z << " )" << std::endl;
}