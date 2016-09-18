#pragma once
#include "Math/Math.h"
#include "Math/Transformation.h"
#include <GLFW/glfw3.h>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 3.0f;
const float SENSITIVTY = 0.25f;
const float ZOOM = 45.0f;


class Camera
{
	enum class  ProjectionType{
		Perspective,
		Orthographic

	};
public:

	Camera();
	Camera(Camera& other);
	~Camera();
	Camera& operator=(Camera& other);	
	void LookAt(Vector3& position, Vector3& up);
	void offSetOrientation(float angle, float pitch);
	

	Vector3 Forward();
	Vector3 Backward();
	Vector3 Right();
	Vector3 Left();
	Vector3 Up();
	Vector3 Down();
	Matrix4 GetMatrix();
	Matrix4 GetProjection();
	Matrix4 GetView();
	void UpdateKey(bool* keys, float);
	void UpdateMouse(GLFWwindow* window, float);

	float OrthoScale;
	
	
	Transformation Transform;
	float FieldOfView;
	float NearPlane;
	float FarPlane;
	float ViewPortAspect;
	ProjectionType Projection;
	
	double previosX;
	double previosY;
	double posX;
	double posY;

private:
	
	
};
