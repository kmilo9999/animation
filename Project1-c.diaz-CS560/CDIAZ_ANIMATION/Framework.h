#pragma once
#include "OpenglGL.h"
#include <GLFW/glfw3.h>
#include "Math/Vector2.h"
#include "Renderer.h"
#include "Common.hpp"
#include <AntTweakBar.h>

class Framework
{
public:
	Framework();
	~Framework();
	void Init();
	void Run();
	GLFWwindow* GetWindow();
	void SetWindow(GLFWwindow*);
	void SetWindowSize(Vector2 v);
	Vector2 GetWindowSize();
	
	void KeyPressedEvent(int key, int action);
	void MouseMoveEvent(double  posx, double posy);
	bool running;
	bool keys[1024];
	bool showBones = false;
	TwBar *myBar;

	Renderer* GetRendered();
	

private:
	void LoadMeshShaders();
	void LoadBonesShaders();
	void InitDebugGui();
	void InitEntities();
	void InitOpengl();
	void InitCamera();
	static void TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods);
	static void TwEventMouseButtonGLFW3(GLFWwindow* window, double xpos, double ypos);

	GLFWwindow* window;
	
	Vector2 WindowSize;
	Renderer* render;
	Shader meshShader;
	Shader bonesShader;
	Vector3 m_position;
	float fps;
	
};

