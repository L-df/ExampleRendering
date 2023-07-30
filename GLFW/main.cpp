#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>


namespace GLFWCallbacksAndInitialization
{

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

    GLFWwindow* CreateAndInitializeGLFWwindow()
    {
        if(!glfwInit())
        {
            fprintf(stderr, "Failed to initialize glfw\n");
            return nullptr;
        }
        
        glfwSetErrorCallback(GLFWErrorCallback);

        GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle, NULL, NULL);

        if(!window)
        {
            fprintf(stderr, "Failed to create glfw window\n");
            glfwTerminate();
            return nullptr;
        }
        
        glfwSetKeyCallback(window, GLFWKeyCallback);

        glfwMakeContextCurrent(window);

        glfwSwapInterval(1);

        // TODO: GLFW framebuffer size callback

        return window;
    }
};

// TODO: Non-renderer specific simple physics sim
// TODO: Window resizing
// TODO (?): Full screen?
// TODO: Make simple physics scene to ARGB for GLFW dispaly

int main(void)
{
	GLFWwindow* window = GLFWCallbacksAndInitialization::CreateAndInitializeGLFWwindow();
	// TODO: Framebuffer size callback

    // TODO: Do the thing in linux that removes the command line if run by file explorer
    fprintf(stdout, "Press Escape to close\n");    

	while(window && !glfwWindowShouldClose(window))
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
