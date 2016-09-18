#include "Framework.h"
#include <stdlib.h>
#include <stdio.h>
#include "Shader.h"
#include "Math/Transformation.h"
#include <windows.h>


#define  TIME_STEP 1.0f/60.0f


void TW_CALL ShowBones(void *clientData);
void TW_CALL SetModel(const void *value, void *clientData);
void TW_CALL GetModel(void *value, void *clientData);


Framework::Framework() :window(), WindowSize(854, 480)
{
	
}


Framework::~Framework()
{
}

void Framework::Init()
{
	InitOpengl();
	InitDebugGui();
	InitEntities();
	InitCamera();
	LoadMeshShaders();
	LoadBonesShaders();
	
}

void Framework::Run()
{
	bool fullscreen = false;
	
	running = true;
	double accumulator = 0;
	double timeaccumulator = 0;
	double frameCounter = 0.0f;
	double prevTime = glfwGetTime();
	float RunningTime, prevRunningTime = 0.0f;
	double m_startTime = (double)GetTickCount();
	/* Loop until the user closes the window */
	glfwSwapInterval(1);
	while (running)
	{
		//Handle window resize
		{
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			WindowSize.x = (float)width;
			WindowSize.y = (float)height;
			glViewport(0, 0, (GLsizei)WindowSize.x, (GLsizei)WindowSize.y);

		}
		{
			if (glfwWindowShouldClose(window))
			{
				running = false;
			}
		}
		

		double currentTime = glfwGetTime();
		double dt = currentTime - prevTime;
		prevTime = currentTime;
		accumulator += dt;
		timeaccumulator += dt;
		render->HandleKeyInput(keys);

		while (accumulator >= TIME_STEP)
		{
			accumulator -= TIME_STEP;
			frameCounter += 1;
			render->GetCamera()->UpdateKey(keys, TIME_STEP);
			render->GetCamera()->UpdateMouse( window,TIME_STEP);
		}

		RunningTime = (float)((double)GetTickCount() - (double)m_startTime) / 1000.0f;
		float elapsedTime = (RunningTime - prevRunningTime) + 0.00001f;
		//int fps = (int)(1.f / elapsedTime);
		prevRunningTime = RunningTime;

		render->UpdateEntities(meshShader, RunningTime);
	
		render->Prepare();
		render->SetScreenWidth(WindowSize.x);
		render->SetScreenHeight(WindowSize.y);
		render->RenderModel(meshShader);

		
		if (showBones)
		{
			render->RenderBones(bonesShader);
		}

		frameCounter += 1;
		
		if (timeaccumulator > 1.0f)
		{
			fps = (float)(frameCounter / timeaccumulator );
			timeaccumulator -= 1.0f;
			frameCounter = 0.0f;
		}
		
		
		TwDraw();
		
		glfwSwapBuffers(window);
		
		/* Poll for and process events */
		glfwPollEvents();

	}

	glfwDestroyWindow(window);
	TwTerminate();
	delete render;
}

void Framework::InitOpengl()
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	}


	glEnable(GL_DEPTH_TEST);
}

void Framework::InitCamera()
{
	Camera* camera = new Camera();
	camera->Transform.Position = Vector3(0, 90.0f, 70.0f);
	//camera->Transform.Position = Vector3(0, 100, 100.0f);
	camera->LookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	camera->FieldOfView = 45;

	render->SetCamera(camera);
}

void Framework::LoadMeshShaders()
{
	meshShader.CreateProgram();
	meshShader.CreateShaderFromFile("Shaders/vertexShader.glsl", ShaderType::VERTEX_SHADER);
	meshShader.CreateShaderFromFile("Shaders/fragmentShader.glsl", ShaderType::FRAGMENT_SHADER);
	meshShader.LinkProgram();

	if (!meshShader.IsLinked())
		throw runtime_error("Shader not Linked Properly");

	meshShader.Use();

	meshShader.SetBonesLocation();
	meshShader.Unuse();
}

void Framework::LoadBonesShaders()
{
	bonesShader.CreateProgram();
	bonesShader.CreateShaderFromFile("Shaders/BonesVertex.glsl", ShaderType::VERTEX_SHADER);
	bonesShader.CreateShaderFromFile("Shaders/BonesFragment.glsl", ShaderType::FRAGMENT_SHADER);
	bonesShader.LinkProgram();

	if (!bonesShader.IsLinked())
		throw runtime_error("Shader not Linked Properly");

}

void Framework::InitEntities()
{
	render = new Renderer();

	render->InitMesh();

}
GLFWwindow* Framework::GetWindow()
{
	return window;
}

void Framework::SetWindowSize(Vector2 v)
{
	WindowSize = v;
}

Vector2 Framework::GetWindowSize()
{
	return WindowSize;
}

void Framework::SetWindow(GLFWwindow* w)
{
	window = w;
}

void Framework::KeyPressedEvent(int key, int action)
{
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void Framework::MouseMoveEvent(double  posx, double posy)
{
	render->GetCamera()->posX = posx;
	render->GetCamera()->posY = posy;
}

Renderer* Framework::GetRendered()
{
	return render;
}

void TW_CALL ShowBones(void *clientData)
{
	Framework *scene = static_cast<Framework *>(clientData); 
	scene->showBones = !scene->showBones;
}

void TW_CALL SetModel(const void *value, void *clientData)
{
	Framework *scene = static_cast<Framework *>(clientData);
	int val = *(int*)value;
	scene->GetRendered()->SetSelectedMesh(val);	
	scene->GetRendered()->GetMesh()->selectedAnimation = 0;
}

void TW_CALL GetModel(void *value, void *clientData)
{
	Framework *scene = static_cast<Framework *>(clientData);
	*(int*)value = scene->GetRendered()->GetSelectedMesh();
}

void TW_CALL SetAnimation(const void *value, void *clientData)
{
	Framework *scene = static_cast<Framework *>(clientData);
	int val = *(int*)value;
	int numAni = scene->GetRendered()->GetMesh()->numAnimations;
	if (val <= numAni)
	{
		scene->GetRendered()->GetMesh()->selectedAnimation = val - 1;
	}
	
}

void TW_CALL GetAnimation(void *value, void *clientData)
{
	Framework *scene = static_cast<Framework *>(clientData);
	*(int*)value = scene->GetRendered()->GetMesh()->selectedAnimation;
}


void Framework::TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods)
{ 
	if (TwEventMouseButtonGLFW(button, action))
		return;
}
void Framework::TwEventMouseButtonGLFW3(GLFWwindow* window, double xpos, double ypos)
{
	if (TwMouseMotion((int)xpos, (int)ypos))
		return;
}

void Framework::InitDebugGui()
{
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize((int)WindowSize.x, (int)WindowSize.y);
	glfwSetMouseButtonCallback(window, Framework::TwEventMouseButtonGLFW3);
	glfwSetCursorPosCallback(window, Framework::TwEventMouseButtonGLFW3);;
	myBar = TwNewBar("Debug");
	TwDefine(" Debug size='300 150' ");
	
	
	TwAddVarRO(myBar, "FPS", TW_TYPE_FLOAT, &fps, " label='FPS' ");

	TwAddButton(myBar, "Show Bones", (TwButtonCallback)ShowBones, this, " label='Show Bones' ");
	TwAddVarCB(myBar, "Model", TwDefineEnum("Model", NULL, 0),
		SetModel, GetModel, this,
		" enum='0 {Lizzard}, 1 {Police}' ");
	
	TwAddVarCB(myBar, "Animation", TwDefineEnum("Animation", NULL, 0),
		SetAnimation, GetAnimation, this,
		" enum=' 1 {1}, 2 {2} , 3 {3}' ");
	
	TwAddButton(myBar, "Comment1", NULL, NULL, " label='USE AWSD KEYS TO MOVE THE CAMERA'");
	TwAddButton(myBar, "Comment2", NULL, NULL, " label='PRESS THE MOUSE MIDDLE BUTTON AND'"); 
	TwAddButton(myBar, "Comment3", NULL, NULL, " label='DRAG IT TO ROTATE THE VIEW'");
	
	
}