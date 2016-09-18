#pragma once
#include "Vector2.h"
#include <iostream>
class Vector3
{
public:
	Vector3();
	Vector3(float x, float y, float z);
	Vector3( const Vector3& vec);
	Vector3( Vector2& vec);
	Vector3( Vector2& vec, float z);
	~Vector3();

	void Zero();
	float GetX() const;
	void SetX(float nx);
	float GetY() const;
	void SetY(float ny);
	float GetZ() const;
	void SetZ(float nz);


	Vector3& operator=(  Vector3& vec);
	Vector3 operator+( Vector3& vec);
	Vector3 operator-( Vector3& vec);
	Vector3 operator+=(Vector3& vec);
	Vector3& operator*( float c);
	float operator[](int index);

	

	union
	{
		struct
		{
			float x, y, z;
		};
		float data[3];
	};

private:
	

	
};

Vector3 operator*(float c, Vector3& vec);
Vector3 operator*( Vector3& a,  Vector3& b);
Vector3 operator/(Vector3& a, Vector3& b);
std::ostream& operator<<(std::ostream& os, Vector3& q);