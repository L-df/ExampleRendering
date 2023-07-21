#include <stdio.h>
#include <stdlib.h>

// TODO: manage loading manually
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main(void)
{
	if(!glfwInit())
	{
		fprintf(stderr, "Failed to initialize glfw\n");
		return 1;
	}
	


	glfwTerminate();
	return 0;
}
