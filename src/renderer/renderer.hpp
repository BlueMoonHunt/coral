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
        void createSwapchain(VkSurfaceFormatKHR surfaceFormat, VkPresentModeKHR presentMode);
        void createImageView(VkSurfaceFormatKHR);
    private:
        VkAllocationCallbacks* allocator;
        VkInstance instance;
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        VkSurfaceKHR swapchainSurface;
        uint32_t queueFamily;
        VkQueue queue;
        VkSwapchainKHR swapchain;

        uint32_t swapChainImageCount;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;

        //

        RendererProperties properties;
    };
} // namespace coral
