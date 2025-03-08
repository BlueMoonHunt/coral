#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "base.hpp"

struct GLFWwindow;

namespace coral {

    struct RendererProperties {
        ivec2 framebufferSize;
        FLAG flags = CoralRendererFlag_None;
    };

    struct Swapchain {
        VkSwapchainKHR handle;
        VkSurfaceKHR surface;
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
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        uint32_t queueFamily;
        VkQueue queue;
        
        RendererProperties properties;
        Swapchain swapchain;
    };
} // namespace coral
