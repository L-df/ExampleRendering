#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>



struct GLFWPixel
{
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
};

static_assert(sizeof(GLFWPixel) == 3 * sizeof(unsigned char));

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

template<int Width, int Height>
struct SimplePixelArray
{
    GLFWPixel PixelData[Width*Height];

    GLFWPixel& GetPixel(int x, int y) { return PixelData[x*Width + y]; };
};

template<int Width, int Height>
constexpr SimplePixelArray<Width, Height> GenerateSimpleColor(int r, int g, int b)
{
    SimplePixelArray<Width, Height> ReturnArray;

    for(int index = 0; index < Width * Height; index++)
    {
        ReturnArray.PixelData[index].r = r;
        ReturnArray.PixelData[index].g = g;
        ReturnArray.PixelData[index].b = b;
    }

    return ReturnArray;
}

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

    constexpr SimplePixelArray<WindowWidth, WindowHeight> DisplayPixels =
             GenerateSimpleColor<WindowWidth, WindowHeight>(255, 255, 255);    

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
        glDrawPixels(WindowWidth, WindowHeight, GL_RGB, GL_UNSIGNED_BYTE, &DisplayPixels.PixelData);
	
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
