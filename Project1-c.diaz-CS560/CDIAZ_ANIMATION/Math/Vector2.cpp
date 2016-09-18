#include "Vector2.h"
#include <cmath>
#include <stdexcept>


Vector2::Vector2()
{
}

Vector2::Vector2(float nx, float ny) : x(nx), y(ny)
{

}


Vector2::Vector2(const Vector2& vec) :x(vec.x), y(vec.y)
{

}

Vector2::~Vector2()
{
}

void Vector2::Zero()
{
	x = 0.0f;
	y = 0.0f;

}

float Vector2::GetX() const
{
	return x;
}

void Vector2::SetX(float nx)
{
	x = nx;
}

float Vector2::GetY() const
{
	return y ;
}

void Vector2::SetY(float ny)
{
	y = ny;
}

Vector2& Vector2::operator=( const Vector2& vec)
{

	this->x = vec.x;
	this->y = vec.y;
	
	return *this;
}

Vector2 Vector2::operator+(const Vector2& vec2)
{
	Vector2 result;
	result.x = this->x + vec2.GetX();
	result.y = this->x + vec2.GetY();
	return result;
}

Vector2 Vector2::operator-(const Vector2& vec2)
{
	Vector2 result;
	result.x = this->x - vec2.GetX();
	result.y = this->x - vec2.GetY();
	return result;
}



Vector2 Vector2::operator*(const float c)
{
	Vector2 result;
	result.x = this->x * c;
	result.y = this->x * c;
	return result;
}


void Vector2::Normalize()
{
	float normalizeFactor = (float)sqrt((x *x) + (y * y));
	x = (x / normalizeFactor);
	y = (y / normalizeFactor);
}

float Vector2::Dot(const Vector2 vec2)
{
	return (x* vec2.GetX()) + (y* vec2.GetY());

}

float& Vector2::operator[](int index)
{
	return data[index];
}