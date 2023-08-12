#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <math.h>


struct GLFWPixel
{
public:
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
public:
    GLFWPixel PixelData[Width*Height] = {};

    constexpr GLFWPixel& GetPixel(int x, int y) { return PixelData[x*Width + y]; };
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

template<typename NumericType = int>
struct Vector2
{
    NumericType X;
    NumericType Y;
};

struct Sphere
{
    union
    {
        Vector2<int> Center;
        int C[2];
    };
    int Radius;
};

constexpr Sphere CreateSphere(int CenterX, int CenterY, int Radius)
{
    Sphere ReturnObject = Sphere();

    ReturnObject.Center.X = CenterX;
    ReturnObject.Center.Y = CenterY;
    ReturnObject.Radius = Radius;
    
   return ReturnObject;
}

template<int Width, int Height>
constexpr SimplePixelArray<Width, Height> 
    GenerateSimpleColor(
    const int& r, const int& g, const int& b,
    const SimplePixelArray<Width, Height>& BaseImageToChange, 
    const Sphere& SphereToDraw)
{
    SimplePixelArray<Width, Height> ReturnArray = BaseImageToChange;

    for(int HeightIndex = 0; HeightIndex < Height; HeightIndex++)
    {
        int HeightDifference = std::abs(HeightIndex - SphereToDraw.Center.Y);
        
        for(int WidthIndex = 0; WidthIndex < Width; WidthIndex++)
        {
            int WidthDifference = std::abs(WidthIndex - SphereToDraw.Center.X);
            if(HeightDifference + WidthDifference < SphereToDraw.Radius)
            {
                ReturnArray.GetPixel(WidthIndex, HeightIndex).r = r;
                ReturnArray.GetPixel(WidthIndex, HeightIndex).g = g;
                ReturnArray.GetPixel(WidthIndex, HeightIndex).b = b;
            }
        }
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

    constexpr SimplePixelArray<WindowWidth, WindowHeight> OriginalDisplayPixels =
             GenerateSimpleColor<WindowWidth, WindowHeight>(255, 255, 255);    

    constexpr Sphere SphereToDrawOne = CreateSphere(WindowWidth * 0.33, WindowHeight * 0.5, WindowWidth * 0.1);
    

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
    
        constexpr SimplePixelArray<WindowWidth, WindowHeight> DisplayPixels 
            = GenerateSimpleColor<WindowWidth, WindowHeight>(125, 125, 125, OriginalDisplayPixels, SphereToDrawOne);        

        glDrawPixels(WindowWidth, WindowHeight, GL_RGB, GL_UNSIGNED_BYTE, &DisplayPixels.PixelData);
	
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
