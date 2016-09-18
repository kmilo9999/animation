#include "Camera.h"
#include <cmath>


Camera::Camera() :Transform(), FieldOfView(50.0f), NearPlane(0.1f), FarPlane(1000.0f), OrthoScale(1.0f),
					ViewPortAspect(4.0f / 3.0f), Projection(ProjectionType::Perspective)
{
}

Camera::Camera(Camera& other) : Transform(other.Transform), FieldOfView(other.FieldOfView), OrthoScale(1.0f),
								NearPlane(other.NearPlane), FarPlane(other.FarPlane),
								ViewPortAspect(other.ViewPortAspect), Projection(other.Projection)
{

}

Camera::~Camera()
{
	
}

void Camera::LookAt(Vector3& position, Vector3& up)
{
	Vector3 pos = Transform.Position;
	if (fabs(Math::Dot(pos, up) - 1.0f) > 0.0001f)
	{
		Matrix4	mat = Math::LookAt(pos, position, up);
		Quaternion q = Math::Matrix4ToQuaternion(mat);
		Transform.Orientation = q;
	}
}

void Camera::offSetOrientation(float yaw, float pitch)
{
	Quaternion yawRotation = Math::AngleRadiansAxis(yaw, Vector3(0,1,0));
	Quaternion pitchRotation = Math::AngleRadiansAxis(pitch, Right());

	Transform.Orientation = Transform.Orientation  * pitchRotation * yawRotation;
}

Vector3 Camera::Forward()
{
	return Math::conjugate(Transform.Orientation) * Vector3(0, 0, -1);
}

Vector3 Camera::Backward()
{
	return Math::conjugate(Transform.Orientation) * Vector3(0, 0, 1);
}

Vector3 Camera::Right()
{
	return Math::conjugate(Transform.Orientation) * Vector3(1, 0, 0);
}

Vector3 Camera::Left()
{
	return Math::conjugate(Transform.Orientation) * Vector3(-1, 0, 0);
}

Vector3 Camera::Up()
{
	return Math::conjugate(Transform.Orientation) * Vector3(0, 1, 0);
}

Vector3 Camera::Down()
{
	return Transform.Orientation * Vector3(0, -1, 0);
}

Matrix4 Camera::GetMatrix() 
{
	Matrix4 proj = GetProjection();
	Matrix4 view = GetView();
	
	Matrix4 result = proj * view;
	return result;
}

Matrix4 Camera::GetProjection()
{
	Matrix4 proj;
	if (Projection == ProjectionType::Perspective)
	{
		proj = Math::Perspective(FieldOfView, ViewPortAspect, NearPlane, FarPlane);
	}
	else if (Projection == ProjectionType::Orthographic) {
		float distance = 0.5f * (FarPlane - NearPlane);
		proj = Math::Ortho(-OrthoScale * ViewPortAspect, OrthoScale * ViewPortAspect,
			-OrthoScale, OrthoScale,-distance, distance);
	}
	return proj;
}

Matrix4 Camera::GetView()
{
	Matrix4 view;

	view = Math::Scale(Vector3(1, 1, 1) / Transform.Scale)
		* Math::QuaterinionToMatrix(Transform.Orientation)
	* Math::Translate((-1)* Transform.Position);


	return view;
}

void Camera::UpdateKey(bool* keys, float dt)
{
	float camVel = 50.0f;
	Vector3 velDir;
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		velDir += Forward();
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		velDir += Backward();
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		velDir += Left();
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		velDir += Right();
	}

	if (Math::Length(velDir) > 0)
	{
		velDir = Math::Normalize(velDir);
	}

	Transform.Position += camVel * velDir * dt;
}

void Camera::UpdateMouse(GLFWwindow* window,float dt)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
	if (state == GLFW_PRESS)
	{
		const float mouseSensitivity = 1.5f;

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		double posiX, posiY;
		glfwGetCursorPos(window, &posiX, &posiY);
		posiX -= width / 2;
		posiY -= height / 2;

		float xToradians = ((float)posiX * dt) * (float)(M_PI / 180);
		float yToradians = ((float)posiY * dt) * (float)(M_PI / 180);

		offSetOrientation(mouseSensitivity * (xToradians),
			mouseSensitivity * (yToradians));



		glfwSetCursorPos(window, width / 2, height / 2);
	}
	
	

}