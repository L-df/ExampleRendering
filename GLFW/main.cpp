#include <stdio.h>
#include <stdlib.h>

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN 1
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "DrawingPixels.h"

static const int WindowWidth = 640;
static const int WindowHeight = 480;
static const char WindowTitle[] = "Example Window Title\0";


namespace VulkanCallbacksAndInitialization
{
    constexpr VkApplicationInfo DefaultApplicationInfo()
    {
        VkApplicationInfo ReturnDefaultInfo = {};

        ReturnDefaultInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        ReturnDefaultInfo.pApplicationName = "Hello Triangle";
        ReturnDefaultInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        ReturnDefaultInfo.pEngineName = "No Engine";
        ReturnDefaultInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        ReturnDefaultInfo.apiVersion = VK_API_VERSION_1_0;
    
        return ReturnDefaultInfo;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT Severity, 
        VkDebugUtilsMessageTypeFlagsEXT Type,
        const VkDebugUtilsMessengerCallbackDataEXT* CallbackData, /* ? */
        void* UserData  /* ? */)
    {
        // NOTE: Should I swap this to std::cout/std::cerr?
        fprintf(stderr, "Vulkan Validation Layer: ");
        fprintf(stderr, "%s", CallbackData->pMessage);
        fprintf(stderr, "\n");
        
        return VK_FALSE; 
    }    

    // TODO: Pass as parameter
    VkDebugUtilsMessengerEXT GlobalVulkanDebugMessenger = {};

    VkInstance VulkanInit()
    {
        // TODO: Save to some type of struct that can be memory managed on main with stack allocation
        constexpr VkApplicationInfo VulkanApplicationInfo = DefaultApplicationInfo();
        VkInstanceCreateInfo VulkanCreateInfo = {};
        VulkanCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        VulkanCreateInfo.pApplicationInfo = &VulkanApplicationInfo;
        
        // TODO: Start moving the GLFW init in with the Vulkan init rather than two seperate functions
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = 0;
        
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        VulkanCreateInfo.enabledExtensionCount = glfwExtensionCount;
        VulkanCreateInfo.ppEnabledExtensionNames = glfwExtensions;

        VulkanCreateInfo.enabledLayerCount = 0;

        // TODO: Required Extensions        

        VkInstance VulkanInstance = {};        

        // NOTE: nullptr here is used for custom memory allocation - look into docs for making custom allocators    
        VkResult VulkanInitializationResult = vkCreateInstance(&VulkanCreateInfo, nullptr, &VulkanInstance);

        switch(VulkanInitializationResult)
        {
            case VK_SUCCESS: { break; };
            // TODO: More explicit error output
            default: { fprintf(stderr, "Failed To Initialize Vulkan\n"); return NULL; };
        }       
        
        VkDebugUtilsMessengerCreateInfoEXT CreateDebugMessengerInfo = {};
        CreateDebugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        CreateDebugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT 
                                    | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT 
                                    | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        CreateDebugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT 
                                    | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 
                                    | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        CreateDebugMessengerInfo.pfnUserCallback = VulkanCallbacksAndInitialization::VulkanDebugCallback;
        CreateDebugMessengerInfo.pUserData = nullptr; // would be passed to debug callback

        auto ExtensionFunctionForCallback = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(VulkanInstance, "vkCreateDebugUtilsMessengerEXT");
        if(ExtensionFunctionForCallback)
        {
            // TODO: nullptr here is the custom allocation
            VkResult SettingErrorCallbackResult = ExtensionFunctionForCallback(VulkanInstance, &CreateDebugMessengerInfo, nullptr, &GlobalVulkanDebugMessenger);
            
            switch(VulkanInitializationResult)
            {
                case VK_SUCCESS: { break; };
                // TODO: More explicit error output
                default: { fprintf(stderr, "Failed To Set VulkanCallback\n"); break; };
            }          
        }
        else
        {
            fprintf(stderr, "Failed To Set Vulkan Debug Callback\n");
            // return NULL; // TODO: Should I not use vulkan if I can't get errors easily?  
            
        }

        return VulkanInstance;
    }
};


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

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle, NULL, NULL);

        if(!window)
        {
            fprintf(stderr, "Failed to create glfw window\n");
            glfwTerminate();
            return nullptr;
        }
        
        glfwSetKeyCallback(window, GLFWKeyCallback);

        // TODO: Add back in the Make window current when vulkan is done being initialized
        // glfwMakeContextCurrent(window);

        // glfwSwapInterval(1);

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

    VkInstance VulkanInstance = VulkanCallbacksAndInitialization::VulkanInit();

	// TODO: Framebuffer size callback

    // TODO: Do the thing in linux that removes the command line if run by file explorer
    fprintf(stdout, "Press Escape to close\n");    

    constexpr SimplePixelArray<WindowWidth, WindowHeight> OriginalDisplayPixels =
             GenerateSimpleColor<WindowWidth, WindowHeight>(255, 255, 255);    

    Sphere SphereToDraw = CreateSphere(Vector2<int>((int)(WindowWidth * 0.33), (int)(WindowHeight * 0.5)), WindowWidth * 0.1, Vector2<int>(5, 2));
    

	while(window && !glfwWindowShouldClose(window))
    {

        // NOTE: It seems GLFW doesn't allow for what I was wanting to do with software rendering
        //        Where I can just pass it the array of pixels for it to give to the OS to display
        //
        //        https://www.glfw.org/GLFWReference27.pdf
        //        Looking at the table of contents        
        // TODO: Just skip directly to VULKAN and remove -lGL from build script
        
        // fprintf(stdout, "Starting to go through full loop\n");
        SimplePixelArray<WindowWidth, WindowHeight> DisplayPixels 
            = GenerateSimpleColor<WindowWidth, WindowHeight>(125, 125, 125, OriginalDisplayPixels, SphereToDraw);        
     
        // TODO: Vulkan Draw Pixels   
        // fprintf(stdout, "Drawing Pixels\n");
        // glDrawPixels(WindowWidth, WindowHeight, GL_RGB, GL_UNSIGNED_BYTE, &DisplayPixels.PixelData);
	
        // fprintf(stdout, "GLFW Polling events\n");
		// glfwSwapBuffers(window);
		glfwPollEvents();

        SphereToDraw = UpdateSphere(SphereToDraw, 1.0, 0, WindowWidth, 0, WindowHeight); 
    }

    auto VulkanDestroyCallback = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(VulkanInstance, "vkDestroyDebugUtilsMessengerEXT");
    if (VulkanDestroyCallback)
    {
        VulkanDestroyCallback(VulkanInstance, VulkanCallbacksAndInitialization::GlobalVulkanDebugMessenger, nullptr);
    }
    vkDestroyInstance(VulkanInstance, nullptr);        
    // TODO: Should the shutdown be moved to their own functions in the namespaces?    

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
