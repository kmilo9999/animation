#pragma once
#include "Vector3.h"
#include <iostream>

class Quaternion
{
public:
	struct EulerAngles{

		float pitch;
		float yaw;
		float roll;
	};

	Quaternion();	
	
	Quaternion(float, float, float , float);
	Quaternion(const Quaternion& q);
	Quaternion(Vector3& v,float w);
	~Quaternion();

	Quaternion operator-() ;
	Quaternion operator+(Quaternion& q1);
	Quaternion operator-(Quaternion& q1);
	Quaternion& operator*(float s);
	Quaternion operator/(float s);
	Quaternion& operator=(Quaternion&);
	

	
	

	Vector3 v;
	float w;
private:
	
};

Quaternion operator*(Quaternion& q1, Quaternion& q2);
Quaternion operator*(float q1, Quaternion& q2);

std::ostream& operator<<(std::ostream& os, Quaternion& dt);