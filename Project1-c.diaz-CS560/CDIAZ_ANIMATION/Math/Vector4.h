#pragma once
#include "Vector3.h"
#include "Vector2.h"
class Vector4
{
public:
	Vector4();
	Vector4(float x, float y, float z, float w);
	Vector4( const Vector4& vec);
	Vector4( Vector3& vec);
	Vector4( Vector3& vec, float w);
	Vector4( Vector2& vec);
	Vector4( Vector2& vec, float z, float w);

	~Vector4();
	void Zero();
	float GetX() const;
	void SetX(float nx);
	float GetY() const;
	void SetY(float ny);
	float GetZ() const;
	void SetZ(float nz);
	float GetW() const;
	void SetW(float nz);


	Vector4& operator=( Vector4 vec);
	Vector4 operator+( Vector4& vec);
	Vector4 operator-( Vector4& vec);
	Vector4 operator*( float c);
	float& operator[](int index);
	

	union
	{
		struct
		{
			float x, y, z, w;
		};
		float data[4];
	};
private:
	
};

Vector4 operator*(Vector4& a, Vector4& b);
std::ostream& operator<<(std::ostream& os, Vector4& dt);