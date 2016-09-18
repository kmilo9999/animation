#pragma once
#define _USE_MATH_DEFINES

#include "Vector3.h"
#include "Matrix4.h"
#include "../Camera.h"
#include "Quaternion.h"
#include "Transformation.h"
#include <cmath>


#define TAU 6.2832
#define EPSILON 0.00001

class Math
{
public:
	Math();
	~Math();


	static Matrix4 Translate(Vector3& translation);
	static Matrix4 Rotate(float angle, Vector3& axis);
	static Matrix4 Scale(Vector3& scale);

	static Matrix4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar);
	static Matrix4 Perspective(const float& fovy, float aspect, float zNear, float zFar);
	static Matrix4 LookAt( Vector3& eye,Vector3& center,Vector3& up);
	

	static Vector3 Normalize(const Vector3& vec);
	static Vector4 Normalize(const Vector4& );

	static float Dot(const Vector3 vec, const Vector3 vec2);
	static float Dot(const Vector4 vec, const Vector4 vec2);

	static Vector3 Cross(const Vector3 vec1, const Vector3 vec2);
	static  float Length(const Vector3);
	static float LengthSquare(const Vector3);
	
	static float DegreesToRadians(float degrees);
	static float RadiansToDegrees(float radians);
	static float Dot(Quaternion& q1, Quaternion& q2);
	static Quaternion Nomalize(Quaternion& q1);
	static Quaternion cross(const Quaternion& a, const Quaternion& b);
	static Quaternion conjugate(const Quaternion& q);

	static Vector3 Axis( Quaternion& q);
	static Quaternion AngleDegreesAxis(float degress, Vector3& axis);
	static Quaternion AngleRadiansAxis(float degress, Vector3& axis);
	static Quaternion inverse(Quaternion& q); 
	static float angle(const Quaternion& q);
	static float roll(const Quaternion& q);
	static float pitch(const Quaternion& q);
	static float yaw(const Quaternion& q);
	static Quaternion::EulerAngles quaternionToEulerAngles(const Quaternion& q);

	static Matrix4 QuaterinionToMatrix( Quaternion& q);
	static Quaternion Matrix4ToQuaternion( Matrix4& m);
	
	static float LenghtSquared(Quaternion& q);	
	static float Lenght(Quaternion& q);

	static Matrix4 Inverse( Matrix4& m);
	
};

Vector3 operator*(Quaternion& q, Vector3& v);
Transformation operator / (Transformation& t1, Transformation& t2);
Transformation inverse(Transformation t);
Matrix4 TranformationToMatrix4(Transformation& T);
Transformation operator * (Transformation& t1, Transformation& t2);