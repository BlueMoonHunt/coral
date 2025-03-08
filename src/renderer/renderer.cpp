#include "renderer.hpp"

#include <string>
#include <vector>
#include <print>

#include "application.hpp"

#include <GLFW/glfw3.h>

#ifdef NDEBUG
#define VK_CHECK(expr) {}
#else
#define VK_CHECK(expr)                                    \
{                                                         \
    VkResult result = expr;                               \
    if (result != VK_SUCCESS)                             \
    {                                                     \
        fprintf(stderr, "Vulkan Error: %s returned %d at %s:%d\n", \
                #expr, result, __FILE__, __LINE__);      \
        exit(EXIT_FAILURE);                               \
    }                                                     \
}
#endif

namespace coral {
    void Renderer::init(GLFWwindow* window) {
        createInstance();
        selectPhysicalDevice();
        createSurface(window);
        selectQueueFamily();
        createDevice();
        getQueue();
    }

    void Renderer::shutdown() {
        for (uint32_t i = 0; i < swapChainImageCount; ++i) {
            vkDestroyImageView(device, swapChainImageViews[i], allocator);
        }

        vkDestroySwapchainKHR(device, swapchain, allocator);
        vkDestroyDevice(device, allocator);
        vkDestroySurfaceKHR(instance, swapchainSurface, allocator);
        vkDestroyInstance(instance, allocator);
    }

    void Renderer::beginFrame() {
        glfwPollEvents();
        if (properties.flags & CoralRendererFlag_RecreateSwapChain) {
            properties.flags ^= CoralRendererFlag_RecreateSwapChain;
            VkSurfaceFormatKHR surfaceFormat = getSurfaceFormat();
            VkPresentModeKHR presentMode = getPresentMode();
            createSwapchain(surfaceFormat, presentMode);
            createImageView(surfaceFormat);
        }
        // uint32_t imageIndex;
        // VK_CHECK(vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, NULL, NULL, &imageIndex));

        // VkPresentInfoKHR presentInfo{
        //     .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        //     .swapchainCount = 1,
        //     .pSwapchains = &swapchain,
        //     .pImageIndices = &imageIndex,
        // };

        // VkResult result = vkQueuePresentKHR(queue, &presentInfo);
        // if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        //     properties.flags |= CoralRendererFlag_RecreateSwapChain;
    }

    void Renderer::endFrame() {
    }

    void Renderer::createInstance() {
        uint32_t requiredExtensionCount;
        const char** requiredExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionCount);

        const VkApplicationInfo applicationInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = Application::Get()->getProperties().name,
            .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
            .pEngineName = "coral",
            .engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
            .apiVersion = VK_API_VERSION_1_3,
        };

        const VkInstanceCreateInfo instanceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &applicationInfo,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = requiredExtensionCount,
            .ppEnabledExtensionNames = requiredExtensions,
        };

        VK_CHECK(vkCreateInstance(&instanceCreateInfo, allocator, &instance));
    }

    void Renderer::selectPhysicalDevice() {
        uint32_t physicalDeviceCount = 0;
        VK_CHECK(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr));
        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        VK_CHECK(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()));
        physicalDevice = physicalDevices[0];
    }

    void Renderer::createSurface(GLFWwindow* window) {
        VK_CHECK(glfwCreateWindowSurface(instance, window, allocator, &swapchainSurface));
    }

    void Renderer::selectQueueFamily() {
        uint32_t count;
        queueFamily = UINT32_MAX;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies;
        queueFamilies.resize(count * sizeof(VkQueueFamilyProperties));
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, queueFamilies.data());

        for (uint32_t i = 0; i < queueFamilies.size(); ++i)
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && glfwGetPhysicalDevicePresentationSupport(instance, physicalDevice, i)) {
                queueFamily = i;
                break;
            }

        if (queueFamily == UINT32_MAX)
            throw "could't find suitable queue family";
    }

    void Renderer::createDevice() {
        const float queuePriorities = 1.0f;
        const char* extensionName = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

        const VkDeviceQueueCreateInfo deviceQueueInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = queueFamily,
            .queueCount = 1,
            .pQueuePriorities = &queuePriorities,
        };

        const VkDeviceCreateInfo deviceInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &deviceQueueInfo,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = 1,
            .ppEnabledExtensionNames = &extensionName,
            .pEnabledFeatures = nullptr,
        };


        vkCreateDevice(physicalDevice, &deviceInfo, allocator, &device);
    }

    void Renderer::getQueue() {
        vkGetDeviceQueue(device, queueFamily, 0, &queue);
    }

    VkSurfaceFormatKHR Renderer::getSurfaceFormat() {
        uint32_t formatCount;
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, swapchainSurface, &formatCount, nullptr));
        std::vector<VkSurfaceFormatKHR> surfaceFormats;
        surfaceFormats.resize(formatCount * sizeof(VkSurfaceFormatKHR));
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, swapchainSurface, &formatCount, surfaceFormats.data()));

        for (auto format : surfaceFormats)
            if (format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR && format.format == VK_FORMAT_B8G8R8A8_SRGB)
                return format;

        return surfaceFormats[0];
    }

    VkPresentModeKHR Renderer::getPresentMode() {
        uint32_t presentModeCount;
        VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, swapchainSurface, &presentModeCount, nullptr));
        std::vector<VkPresentModeKHR> presentModes;
        presentModes.resize(presentModeCount * sizeof(VkPresentModeKHR));
        VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, swapchainSurface, &presentModeCount, presentModes.data()));


        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;;

        for (auto mode : presentModes)
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
                return mode;

        return presentMode;
    }

    void Renderer::createSwapchain(VkSurfaceFormatKHR surfaceFormat, VkPresentModeKHR presentMode) {

        VkSurfaceCapabilitiesKHR capabilities;
        VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, swapchainSurface, &capabilities));

        VkSwapchainKHR oldSwapchain;

        const VkSwapchainCreateInfoKHR swapchainInfo{
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .surface = swapchainSurface,
            .minImageCount = std::clamp(3u, capabilities.minImageCount, capabilities.maxImageCount),
            .imageFormat = surfaceFormat.format,
            .imageColorSpace = surfaceFormat.colorSpace,
            .imageExtent = (VkExtent2D){
                .width = std::clamp(capabilities.currentExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
                .height = std::clamp(capabilities.currentExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
            },
            .imageArrayLayers = 1,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices = &queueFamily,
            .preTransform = capabilities.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = presentMode,
            .clipped = true,
            .oldSwapchain = swapchain,
        };

        VK_CHECK(vkCreateSwapchainKHR(device, &swapchainInfo, allocator, &oldSwapchain));
        vkDestroySwapchainKHR(device, swapchain, allocator);
        swapchain = oldSwapchain;
    }

    void Renderer::createImageView(VkSurfaceFormatKHR surfaceFormat) {
        swapChainImages.clear();
        swapChainImageViews.clear();

        VK_CHECK(vkGetSwapchainImagesKHR(device, swapchain, &swapChainImageCount, nullptr));
        swapChainImages.resize(swapChainImageCount * sizeof(VkImage));
        VK_CHECK(vkGetSwapchainImagesKHR(device, swapchain, &swapChainImageCount, swapChainImages.data()));
        swapChainImageViews.resize(swapChainImageCount * sizeof(VkImageView));

        for (uint32_t i = 0; i < swapChainImageCount; ++i) {
            const VkImageViewCreateInfo imageViewInfo{
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .image = swapChainImages[i],
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format = surfaceFormat.format,
                .components = (VkComponentMapping) {},
                .subresourceRange = (VkImageSubresourceRange) {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1,
                },
            };
            VK_CHECK(vkCreateImageView(device, &imageViewInfo, allocator, &swapChainImageViews[i]));
        }
    }

} // namespace coral