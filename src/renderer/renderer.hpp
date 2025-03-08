#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "base.hpp"

struct GLFWwindow;

#ifdef NDEBUG
#define VK_CHECK(expr) {expr;}
#else
#define VK_CHECK(expr)\
{\
    VkResult result = expr;\
    if (result != VK_SUCCESS && result != VK_INCOMPLETE)\
    {\
        fprintf(stderr, "Vulkan Error: %s returned %d at %s:%d\n",\
                #expr, result, __FILE__, __LINE__);\
        exit(EXIT_FAILURE);\
    }\
}
#endif

namespace coral {

    struct RendererProperties {
        ivec2 framebufferSize;
        FLAG flags = CoralRendererFlag_None;
    };

    struct Swapchain {
        VkSwapchainKHR handle;
        uint32_t imageCount;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;
    };

    class Renderer {
    public:
        void init(GLFWwindow* window);
        void shutdown();

        RendererProperties& getProperties() { return properties; }

        void beginFrame();
        void endFrame();
    private:
        void createInstance();
        void selectPhysicalDevice();
        void createSurface(GLFWwindow* window);
        void selectQueueFamily();
        void createDevice();
        void getQueue();
        
        VkSurfaceFormatKHR getSurfaceFormat();
        VkPresentModeKHR getPresentMode();
        void createSwapchain(VkSurfaceFormatKHR& surfaceFormat, VkPresentModeKHR& presentMode);
        void createImageView(VkSurfaceFormatKHR& surfaceFormat);

        void createGraphicsPipeline();
    private:
        VkAllocationCallbacks* allocator;
        VkInstance instance;
        VkSurfaceKHR surface;
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        uint32_t queueFamily;
        VkQueue queue;
        
        RendererProperties properties;
        Swapchain swapchain;
        VkPipeline graphicsPipeline;
    };
} // namespace coral
