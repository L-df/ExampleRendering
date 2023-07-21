#include <stdio.h>
#include <stdlib.h>

// TODO: manage loading manually
#include <GL/glew.h>
#include <GLFW/glfw3.h>


void GLFWErrorCallback(int errorNumber, const char* errorDescription)
{
	fprintf(stderr, "GLFW Error: %s\n", errorDescription);
	return;
}

static const int WindowWidth = 640;
static const int WindowHeight = 480;
static const char WindowTitle[100] = "Example Window Title\0";

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
	}

	glfwTerminate();
	return 0;
}
