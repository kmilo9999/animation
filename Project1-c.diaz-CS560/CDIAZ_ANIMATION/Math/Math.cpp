#include "Math.h"



Math::Math()
{
}


Math::~Math()
{
}



 Matrix4 Math::Translate(Vector3& vector)
{
	Matrix4 result;

	result[3] = Vector4(vector, 1);
	return result;
}

 Matrix4 Math::Rotate(float degrees, Vector3& vector)
{

	float radians = DegreesToRadians(degrees);
	float c = std::cos(radians);
	
	
	float s = std::sin(radians);
	
	


	Vector3 axis = Normalize(vector);
	Vector3 t = (axis * (1.0f - c));

	
	

	Matrix4 rot;
	
	rot[0][0] = c + t[0] * axis[0];
	rot[0][1] = 0 + t[0] * axis[1] + s * axis[2];
	rot[0][2] = 0 + t[0] * axis[2] - s * axis[1];
	rot[0][3] = 0;



	rot[1][0] = 0 + t[1] * axis[0] - s * axis[2];
	rot[1][1] = c + t[1] * axis[1];
	rot[1][2] = 0 + t[1] * axis[2] + s * axis[0];
	rot[1][3] = 0;


	

	rot[2][0] = 0 + t[2] * axis[0] + s * axis[1];
	rot[2][1] = 0 + t[2] * axis[1] - s * axis[0];
	rot[2][2] = c + t[2] * axis[2];
	rot[2][3] = 0;
		
	

	return rot;


}

 Matrix4 Math::Scale(Vector3& v)
{
	Matrix4 result; 
	result[0] = Vector4(v.GetX(), 0, 0, 0);		 
	result[1] = Vector4(0, v.GetY(), 0, 0);
	result[2] = Vector4(0, 0, v.GetZ(), 0);
	result[3] = Vector4(0, 0, 0, 1);
					 
	return result;
}

 Matrix4 Math::Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4 result;

	result[0][0] = 2.0f / (right - left);
	result[1][1] = 2.0f / (top - bottom);
	result[2][2] = -2.0f / (zFar - zNear);
	result[3][0] = -(right + left) / (right - left);
	result[3][1] = -(top + bottom) / (top - bottom);
	result[3][2] = -(zFar + zNear) / (zFar - zNear);

	return result;
 }

 Matrix4 Math::Perspective(const float& fovy, float aspect, float zNear, float zFar)
 {
	 float radians = DegreesToRadians(fovy);
	 const float tanHalfFovy = std::tan(0.5f * radians);

	 Matrix4 result;
	 result.Zero();
	 result[0][0] = 1.0f / (aspect * tanHalfFovy);
	 result[1][1] = 1.0f / (tanHalfFovy);
	 result[2][2] = -(zFar + zNear) / (zFar - zNear);
	 result[2][3] = -1.0f;
	 result[3][2] = -2.0f * zFar * zNear / (zFar - zNear);

	 return result;
 }

 Matrix4 Math::LookAt(Vector3& eye, Vector3& center, Vector3& up)
 {

	 Vector3 f = Normalize(center - eye);
	 Vector3 s = Normalize(Cross(f, up));
	 Vector3 u = Cross(s, f);

	 Matrix4 result;
	 result[0][0] = +s.x;
	 result[1][0] = +s.y;
	 result[2][0] = +s.z;

	 result[0][1] = +u.x;
	 result[1][1] = +u.y;
	 result[2][1] = +u.z;

	 result[0][2] = -f.x;
	 result[1][2] = -f.y;
	 result[2][2] = -f.z;

	 result[3][0] = -Dot(s, eye);
	 result[3][1] = -Dot(u, eye);
	 result[3][2] = +Dot(f, eye);

	 return result;
 }

 Vector3 Math::Normalize( const Vector3& vec)
 {
	 Vector3 result;
	 float normalizeFactor = (float)sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
	 result.x = (vec.x / normalizeFactor);
	 result.y = (vec.y / normalizeFactor);
	 result.z = (vec.z / normalizeFactor);
	 return result;
 }
 Vector4 Math::Normalize( const Vector4& vec)
 {
	 Vector4 result;
	 float normalizeFactor = (float)sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z) + (vec.w * vec.w));
	 result.x = (vec.x / normalizeFactor);
	 result.y = (vec.y / normalizeFactor);
	 result.z = (vec.z / normalizeFactor);
	 result.w = (vec.w / normalizeFactor);
	 return result;
 }

 float Math::Dot(const Vector3 vec, const Vector3 vec2)
 {
	 float result = (vec.x* vec2.x) + (vec.y* vec2.y) + (vec.z * vec2.z);
	 return result;
 }

 Vector3 Math::Cross(const Vector3 vec1, const Vector3 vec2)
 {
	 float a = vec1.y * vec2.z - vec1.z * vec2.y;
	 float  b = vec1.z * vec2.x - vec1.x * vec2.z;
	 float c = vec1.x * vec2.y - vec1.y * vec2.x;

	 Vector3 result(a, b, c);
	 return result;
 }

 float Math::Length(const Vector3 vec)
 {
	 return (float)sqrt(Dot(vec,vec));
 }

 float Math::LengthSquare(const Vector3 vec)
 {
	 return Dot(vec, vec);
 }

 float Math::Dot(const Vector4 vec, const Vector4 vec2)
 {
	 return (vec.x* vec2.x) + (vec.y* vec2.y) + (vec.z * vec2.z) + (vec.w * vec2.w);
 }

 float Math::DegreesToRadians(float degrees)
 {
	 return degrees * float(M_PI / 180);
 }

 float Math::Dot(Quaternion& q1, Quaternion& q2)
 {
	 float result = Math::Dot(q1.v, q2.v); 
		 result += q1.w * q2.w;
	 return result;
 }

 Quaternion Math::Nomalize(Quaternion& q)
 { 
	 Quaternion q1 = q * (1.0f / Lenght(q));
	 return q1;
 }

 Quaternion Math::cross(const Quaternion& a, const Quaternion& b)
 {
	 Quaternion c;
	 c.v.x = a.w * b.v.x + a.v.x * b.w + a.v.y * b.v.z - a.v.z * b.v.y;
	 c.v.y = a.w * b.v.y + a.v.y * b.w + a.v.z * b.v.x - a.v.x * b.v.z;
	 c.v.z = a.w * b.v.z + a.v.z * b.w + a.v.x * b.v.y - a.v.y * b.v.x;
	 c.w = a.w * b.w - a.v.x * b.v.x - a.v.y * b.v.y - a.v.z * b.v.z;

	 return c;
 }

 Quaternion Math::conjugate(const Quaternion& q)
 {
	 Quaternion c;
	 c.v.x = -q.v.x;
	 c.v.y = -q.v.y; 
	 c.v.z = -q.v.z;
	 c.w = q.w;
	 return c;
 }

 Quaternion Math::inverse(Quaternion& q)
 { 
	 Quaternion c;
	 c = conjugate(q) / Dot(q, q);
	 return c;
 }

 float Math::angle(const Quaternion& q) 
 { 
	 float radians =  2.0f * acos(q.w); 
	 return RadiansToDegrees(radians);
 }

 float Math::RadiansToDegrees(float radians)
 {
	 return radians * (float)(180.0f / M_PI);
 }

 Vector3 Math::Axis( Quaternion& q)
 {
	 float s2 = 1.0f - q.w * q.w;

	 if (s2 <= 0.0f)
		 return Vector3{ 0, 0, 1 };

	 float invs2 = 1.0f / sqrt(s2);

	 return q.v * invs2;
 }

 Quaternion Math::AngleDegreesAxis(float degress, Vector3& axis)
 {
	 Quaternion q;
	 float radians = DegreesToRadians(degress);
	 Vector3 a = Normalize(axis);

	 float s = sin(0.5f * radians);

	 
	q.v = a * s;
	 q.w = cos(0.5f * radians);

	 return q;
 }

 Quaternion Math::AngleRadiansAxis(float degress, Vector3& axis)
 {
	 Quaternion q;
	 
	 Vector3 a = Normalize(axis);

	 float s = sin(0.5f * degress);

	 q.v = a * s;
	 q.w = cos(0.5f * degress);

	 return q;
 }

 float Math::roll(const Quaternion& q)
 {
	 return atan2(2.0f * q.v.x * q.v.y + q.v.z * q.w,
		 q.v.x * q.v.x + q.w * q.w - q.v.y * q.v.y - q.v.z * q.v.z);
 }

 float Math::pitch(const Quaternion& q)
 {
	 return atan2(2.0f * q.v.y * q.v.z + q.w * q.v.x,
		 q.w * q.w - q.v.x * q.v.x - q.v.y * q.v.y + q.v.z * q.v.z);
 }

 float Math::yaw(const Quaternion& q)
 {
	 return asin(-2.0f * (q.v.x * q.v.z - q.w * q.v.y));
 }

 Quaternion::EulerAngles Math::quaternionToEulerAngles(const Quaternion& q)
 {
	 return{ pitch(q), yaw(q), roll(q) };
 }
 
 Matrix4 Math::QuaterinionToMatrix(Quaternion& q)
 {
	 Matrix4 mat ;
	 const Quaternion a = Nomalize(q);

	  float xx = a.v.x * a.v.x;
	  float yy = a.v.y * a.v.y;
	  float zz = a.v.z * a.v.z;
	  float xy = a.v.x * a.v.y;
	  float xz = a.v.x * a.v.z;
	  float yz = a.v.y * a.v.z;
	  float wx = a.w * a.v.x;
	  float wy = a.w * a.v.y;
	  float wz = a.w * a.v.z;

	 mat[0][0] = 1.0f - 2.0f * (yy + zz);
	 mat[0][1] = 2.0f * (xy + wz);
	 mat[0][2] = 2.0f * (xz - wy);

	 mat[1][0] = 2.0f * (xy - wz);
	 mat[1][1] = 1.0f - 2.0f * (xx + zz);
	 mat[1][2] = 2.0f * (yz + wx);

	 mat[2][0] = 2.0f * (xz + wy);
	 mat[2][1] = 2.0f * (yz - wx);
	 mat[2][2] = 1.0f - 2.0f * (xx + yy);

	 return mat;
 }
 
/* Quaternion Math::matrix4ToQuaternion(const Matrix4& m)
 {
	
 }*/

 Vector3 operator*(Quaternion& q, Vector3& v)
 {
	 Vector3 t = 2.0f * Math::Cross(q.v, v);
	 return (v + q.w * t + Math::Cross(q.v, t));
 }

 float Math::LenghtSquared(Quaternion& q)
 {
	 float result1 = Math::Dot(q.v, q.v);
	 float result2 = q.w * q.w;
	 float result3 = result1 + result2;
	 return result3;
 }
 float Math::Lenght(Quaternion& q)
 {
	 float result = sqrt(LenghtSquared(q));
	 return result;
 }

 Quaternion Math::Matrix4ToQuaternion( Matrix4& m)
 {
	  float fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
	  float fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
	  float fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
	  float fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

	 int biggestIndex = 0;
	 float fourBiggestSquaredMinus1{ fourWSquaredMinus1 };
	 if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
	 {
		 fourBiggestSquaredMinus1 = fourXSquaredMinus1;
		 biggestIndex = 1;
	 }
	 if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
	 {
		 fourBiggestSquaredMinus1 = fourYSquaredMinus1;
		 biggestIndex = 2;
	 }
	 if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
	 {
		 fourBiggestSquaredMinus1 = fourZSquaredMinus1;
		 biggestIndex = 3;
	 }

	  float biggestVal = sqrt(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
	   float mult = 0.25f / biggestVal;

	 Quaternion q = Quaternion{ 0, 0, 0, 1 };

	 switch (biggestIndex)
	 {
	 case 0:
	 {
		 q.w = biggestVal;
		 q.v.x = (m[1][2] - m[2][1]) * mult;
		 q.v.y = (m[2][0] - m[0][2]) * mult;
		 q.v.z = (m[0][1] - m[1][0]) * mult;
	 }
	 break;
	 case 1:
	 {
		 q.w = (m[1][2] - m[2][1]) * mult;
		 q.v.x = biggestVal;
		 q.v.y = (m[0][1] + m[1][0]) * mult;
		 q.v.z = (m[2][0] + m[0][2]) * mult;
	 }
	 break;
	 case 2:
	 {
		 q.w = (m[2][0] - m[0][2]) * mult;
		 q.v.x = (m[0][1] + m[1][0]) * mult;
		 q.v.y = biggestVal;
		 q.v.z = (m[1][2] + m[2][1]) * mult;
	 }
	 break;
	 case 3:
	 {
		 q.w = (m[0][1] - m[1][0]) * mult;
		 q.v.x = (m[2][0] + m[0][2]) * mult;
		 q.v.y = (m[1][2] + m[2][1]) * mult;
		 q.v.z = biggestVal;
	 }
	 break;
	 default: // Should never actually get here. Just for sanities sake.
	 break;
	 }

	 return q;
 }

 Transformation operator/(Transformation& t1, Transformation& t2)
 {
	 Transformation transform;
	 Quaternion conjugate = Math::conjugate(t2.Orientation);
	 transform.Position = conjugate * (t1.Position - t2.Position) / t2.Scale;
	 transform.Orientation = conjugate * 	t1.Orientation;
	 transform.Scale = conjugate* (t1.Scale / t2.Scale);
	 return transform;

 }
 
 Transformation inverse(Transformation t)
 {
	 Transformation inverse;
	 return inverse / t;
 }


 Matrix4 TranformationToMatrix4(Transformation& t)
 {
	 Matrix4 tl = Math::Translate(t.Position);	
	 Matrix4 r = Math::QuaterinionToMatrix(t.Orientation);
	 Matrix4 s = Math::Scale(t.Scale);
	 Matrix4 result = tl * r * s;
	 return  result;
 }

 Transformation operator*(Transformation& parentspace, Transformation& localspace)
 {
	 Transformation transform;
	 transform.Position = parentspace.Position + parentspace.Orientation * (parentspace.Scale * localspace.Position);
	 transform.Orientation = parentspace.Orientation * localspace.Orientation;
	 transform.Scale = parentspace.Scale * (parentspace.Orientation * localspace.Scale);
	 return transform;
 }

 Matrix4 Math::Inverse( Matrix4& m)
 {
	  float coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	  float coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
	  float coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
	  float coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	  float coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	  float coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];
	  float coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	  float coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
	  float coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	  float coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	  float coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
	  float coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];
	  float coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	  float coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
	  float coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
	  float coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	  float coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
	  float coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

	  Vector4 fac0(coef00, coef00, coef02, coef03 );
	  Vector4 fac1( coef04, coef04, coef06, coef07 );
	  Vector4 fac2(coef08, coef08, coef10, coef11);
	  Vector4 fac3(coef12, coef12, coef14, coef15);
	  Vector4 fac4(coef16, coef16, coef18, coef19);
	  Vector4 fac5(coef20, coef20, coef22, coef23);

	  Vector4 vec0 (m[1][0], m[0][0], m[0][0], m[0][0] );
	  Vector4 vec1 (m[1][1], m[0][1], m[0][1], m[0][1] );
	  Vector4 vec2 (m[1][2], m[0][2], m[0][2], m[0][2] );
	  Vector4 vec3 (m[1][3], m[0][3], m[0][3], m[0][3] );

	  Vector4 inv0 = vec1 * fac0 - vec2 * fac1 + vec3 * fac2;
	  Vector4 inv1 = vec0 * fac0 - vec2 * fac3 + vec3 * fac4;
	  Vector4 inv2 = vec0 * fac1 - vec1 * fac3 + vec3 * fac5;
	  Vector4 inv3 = vec0 * fac2 - vec1 * fac4 + vec2 * fac5;

	  Vector4 signA( +1, -1, +1, -1 );
	  Vector4 signB(-1, +1, -1, +1 );
	  Vector4 temp = inv0 * signA;
	  Vector4 temp2 = inv1 * signB;
	  Vector4 temp3 = inv2 * signA;
	  Vector4 temp4 = inv3 * signB;
	  Matrix4 inverse(temp, temp2, temp3, temp4);

	  Vector4 row0 (inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0] );

	  Vector4 dot0 = m[0] * row0;
	  float dot1 = (dot0[0] + dot0[1]) + (dot0[2] + dot0[3]);

	  float oneOverDeterminant = 1.0f / dot1;


	 return inverse * oneOverDeterminant;

 }