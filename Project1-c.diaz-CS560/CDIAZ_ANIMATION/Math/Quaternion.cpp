#include "Quaternion.h"


Quaternion::Quaternion() :v(0, 0, 0), w(1)
{

}

Quaternion::Quaternion(float x, float y, float z, float w) : v(x, y, z), w(w)
{
}

Quaternion::Quaternion(Vector3& v, float w) : v(v), w(w)
{
	
}

Quaternion::~Quaternion()
{

}


Quaternion::Quaternion(const Quaternion& q) : v(q.v), w(q.w)
{

}

Quaternion Quaternion::operator-() 
{
	Quaternion q;
	q.w = -w;
	q.v = v * (-1.0f);

	return q;
}

Quaternion Quaternion::operator+(Quaternion& q1)
{
	Quaternion q;
	q.w = w + q.w;
	q.v = v + q.v;

	return q;
}

Quaternion Quaternion::operator-(Quaternion& q1)
{
	Quaternion q;
	q.w = w - q.w;
	q.v = v - q.v;

	return q;
}

Quaternion& Quaternion::operator*(float s)
{
	this->v.x *= s;
	this->v.y *= s;
	this->v.z *= s;
	this->w *= s;

	return *this;
}

Quaternion Quaternion::operator/(float s)
{
	Quaternion c ;

	c.v.x /= s;
	c.v.y /= s;
	c.v.z /= s;
	c.w /= s;

	return c;
}

Quaternion& Quaternion::operator=(Quaternion& q)
{
	this->v.x = q.v.x;
	this->v.y = q.v.y;
	this->v.z = q.v.z;
	this->w = q.w;	

	return *this;
}

Quaternion operator*(Quaternion& a, Quaternion& b)
{
	Quaternion c;
	c.v.x = a.w * b.v.x + a.v.x * b.w + a.v.y * b.v.z - a.v.z * b.v.y;
	c.v.y = a.w * b.v.y - a.v.x * b.v.z + a.v.y * b.w + a.v.z * b.v.x;
	c.v.z = a.w * b.v.z + a.v.x * b.v.y - a.v.y * b.v.x + a.v.z * b.w;
	c.w = a.w * b.w - a.v.x * b.v.x - a.v.y * b.v.y - a.v.z * b.v.z;
	
	return c;
}




Quaternion operator*(float s, Quaternion& q2)
{
	Quaternion c;

	c.v.x *= s;
	c.v.y *= s;
	c.v.z *= s;
	c.w *= s;

	return c;
}



std::ostream& operator<<(std::ostream& os, Quaternion& q)
{
	return std::cout << q.v.x << " " << q.v.y << " " << q.v.z << " " << q.w ;
}