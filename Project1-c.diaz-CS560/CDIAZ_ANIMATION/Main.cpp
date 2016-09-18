#include "OpenglGL.h"
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include "Framework.h"

using namespace std;


Framework scene;
bool firstMouse = true;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		scene.running = false;
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else{
		scene.KeyPressedEvent(key, action);
	}


}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	scene.MouseMoveEvent(xpos, ypos);
}

int main(void)
{

	GLFWwindow* window;
	if (!glfwInit())
		return EXIT_FAILURE;

	window = glfwCreateWindow((int)scene.GetWindowSize().x, (int)scene.GetWindowSize().y, "CDIAZ", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	
	scene.SetWindow(window);

	scene.Init();
	scene.Run();
	return 0;
}