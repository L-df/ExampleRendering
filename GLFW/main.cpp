#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

#include "DrawingPixels.h"

static const int WindowWidth = 640;
static const int WindowHeight = 480;
static const char WindowTitle[] = "Example Window Title\0";

namespace GLFWCallbacksAndInitialization
{

    void GLFWErrorCallback(int errorNumber, const char* errorDescription)
    {
        fprintf(stderr, "GLFW Error: %s\n", errorDescription);
        return;
    }

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
// TODO: Move physics update to different file for more complex files

constexpr Sphere UpdateSphere(const Sphere& InSphereToUpdate, const float& TimeStep,
    const int& ReflectMinX, const int& ReflectMaxX, const int& ReflectMinY, const int& ReflectMaxY)
{
    Sphere ReturnSphere = InSphereToUpdate;

    ReturnSphere.Center.X = InSphereToUpdate.Center.X + (int)(InSphereToUpdate.Velocity.X * TimeStep);

    ReturnSphere.Center.Y = InSphereToUpdate.Center.Y + (int)(InSphereToUpdate.Velocity.Y * TimeStep);

    
    if(ReturnSphere.Center.X < ReflectMinX
        || ReturnSphere.Center.X > ReflectMaxX)
    {
        ReturnSphere.Velocity.X = -ReturnSphere.Velocity.X;
    }

    if(ReturnSphere.Center.Y < ReflectMinY
        || ReturnSphere.Center.Y > ReflectMaxY)
    {
        ReturnSphere.Velocity.Y = -ReturnSphere.Velocity.Y;
    }

    return ReturnSphere;
}

int main(void)
{
	GLFWwindow* window = GLFWCallbacksAndInitialization::CreateAndInitializeGLFWwindow();
	// TODO: Framebuffer size callback

    // TODO: Do the thing in linux that removes the command line if run by file explorer
    fprintf(stdout, "Press Escape to close\n");    

    constexpr SimplePixelArray<WindowWidth, WindowHeight> OriginalDisplayPixels =
             GenerateSimpleColor<WindowWidth, WindowHeight>(255, 255, 255);    

    Sphere SphereToDraw = CreateSphere(Vector2<int>((int)(WindowWidth * 0.33), (int)(WindowHeight * 0.5)), WindowWidth * 0.1, Vector2<int>(5, 2));
    

	while(window && !glfwWindowShouldClose(window))
	{
		// TODO: Actually write the sim/rendering instead of just displaying to screen
		//

        // NOTE: It seems GLFW doesn't allow for what I was wanting to do with software rendering
        //        Where I can just pass it the array of pixels for it to give to the OS to display
        //
        //        https://www.glfw.org/GLFWReference27.pdf
        //        Looking at the table of contents        
        // TODO: Just skip directly to VULKAN and remove -lGL from build script
        
        // fprintf(stdout, "Starting to go through full loop\n");
        SimplePixelArray<WindowWidth, WindowHeight> DisplayPixels 
            = GenerateSimpleColor<WindowWidth, WindowHeight>(125, 125, 125, OriginalDisplayPixels, SphereToDraw);        

        // fprintf(stdout, "Drawing Pixels\n");
        glDrawPixels(WindowWidth, WindowHeight, GL_RGB, GL_UNSIGNED_BYTE, &DisplayPixels.PixelData);
	
        // fprintf(stdout, "GLFW Polling events\n");
		glfwSwapBuffers(window);
		glfwPollEvents();

        SphereToDraw = UpdateSphere(SphereToDraw, 1.0, 0, WindowWidth, 0, WindowHeight); 
    }

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
