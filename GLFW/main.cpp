#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>


void GLFWErrorCallback(int errorNumber, const char* errorDescription)
{
	fprintf(stderr, "GLFW Error: %s\n", errorDescription);
	return;
}

static const int WindowWidth = 640;
static const int WindowHeight = 480;
static const char WindowTitle[] = "Example Window Title\0";

void GLFWKeyCallback(GLFWwindow* Window, int PressedKey, int scancode /*?*/,
			int action /*?*/, int mods /*?*/)
{
	if(PressedKey == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(Window, GLFW_TRUE);
	}
}

int main(void)
{
	if(!glfwInit())
	{
		fprintf(stderr, "Failed to initialize glfw\n");
		return 1;
	}
	
	glfwSetErrorCallback(GLFWErrorCallback);

	GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle, NULL, NULL);

	if(!window)
	{
		fprintf(stderr, "Failed to create glfw window\n");
		glfwTerminate();
		return 3;
	}
	
	glfwSetKeyCallback(window, GLFWKeyCallback);

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);
	
	// TODO: Framebuffer size callback

	while(!glfwWindowShouldClose(window))
	{
		
		// TODO: Actually write the sim/rendering instead of just displaying to screen
		//
		
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
