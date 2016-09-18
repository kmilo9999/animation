#pragma once
class Vector2
{
public:
	Vector2();
	Vector2(float x,float y);
	Vector2(const Vector2& vec);
	~Vector2();
	
	void Zero();
	float GetX() const;
	void SetX(float nx);
	float GetY() const;
	void SetY(float ny);
	
	Vector2& operator=( const Vector2& vec2);
	Vector2 operator+(const Vector2& vec2);
	Vector2 operator-(const Vector2& vec2);
	Vector2 operator*(const float c);
	float& operator[](int index);

	void Normalize();
	float Dot(const Vector2 vec2);

	union
	{
		struct
		{
			float x, y;
		};
		float data[2];
	};

private:
	

};

