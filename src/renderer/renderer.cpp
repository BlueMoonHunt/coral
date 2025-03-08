#include "renderer.hpp"

#include <string>
#include <vector>
#include <print>

#include "core/application.hpp"

#include <GLFW/glfw3.h>

namespace coral {
    void Renderer::init(GLFWwindow* window) {
        createInstance();
        selectPhysicalDevice();
        createSurface(window);
        selectQueueFamily();
        createDevice();
        getQueue();

        VkSurfaceFormatKHR surfaceFormat = getSurfaceFormat();
        VkPresentModeKHR presentMode = getPresentMode();

        createSwapchain(surfaceFormat, presentMode);
        createImageView(surfaceFormat);

    }

    void Renderer::shutdown() {
        for (uint32_t i = 0; i < swapchain.imageCount; ++i) {
            vkDestroyImageView(device, swapchain.imageViews[i], allocator);
        }

        swapchain.imageViews.clear();
        swapchain.images.clear();

        vkDestroySwapchainKHR(device, swapchain.handle, allocator);
        vkDestroyDevice(device, allocator);
        vkDestroySurfaceKHR(instance, surface, allocator);
        vkDestroyInstance(instance, allocator);
    }

    void Renderer::beginFrame() {
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
        uint32_t physicalDeviceCount;
        VK_CHECK(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr));

        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        VK_CHECK(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()));

        // Attempt to prefer a discrete GPU
        VkPhysicalDevice discreteGPU = VK_NULL_HANDLE;
        VkPhysicalDevice integratedGPU = VK_NULL_HANDLE;

        for (const auto& device : physicalDevices) {
            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(device, &deviceProperties);
            if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                discreteGPU = device;
                break; // Prioritize the first discrete GPU found
            }
            else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
                integratedGPU = device; // Keep track of an integrated GPU in case no discrete one is found
            }
        }

        if (discreteGPU != VK_NULL_HANDLE) {
            physicalDevice = discreteGPU;
            CORAL_LOG("Using Discrete GPU.", LogLevel::INFO);
        }
        else if (integratedGPU != VK_NULL_HANDLE) {
            physicalDevice = integratedGPU;
            CORAL_LOG("Using Integrated GPU. No Discrete GPU found.", LogLevel::INFO);
        }
        else {
            physicalDevice = physicalDevices[0]; // Fallback to the first device if neither discrete nor integrated is clearly identified
            CORAL_LOG("Using Default GPU. No Discrete or Integrated GPU clearly identified.", LogLevel::INFO);
        }

        VkPhysicalDeviceProperties selectedDeviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &selectedDeviceProperties);
    }

    void Renderer::createSurface(GLFWwindow* window) {
        VK_CHECK(glfwCreateWindowSurface(instance, window, allocator, &surface));
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
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr));
        std::vector<VkSurfaceFormatKHR> surfaceFormats;
        surfaceFormats.resize(formatCount * sizeof(VkSurfaceFormatKHR));
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, surfaceFormats.data()));

        for (auto format : surfaceFormats)
            if (format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR && format.format == VK_FORMAT_B8G8R8A8_SRGB)
                return format;

        return surfaceFormats[0];
    }

    VkPresentModeKHR Renderer::getPresentMode() {
        uint32_t presentModeCount;
        VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr));
        std::vector<VkPresentModeKHR> presentModes;
        presentModes.resize(presentModeCount * sizeof(VkPresentModeKHR));
        VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data()));


        for (auto mode : presentModes)
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
                return mode;

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    void Renderer::createSwapchain(VkSurfaceFormatKHR& surfaceFormat, VkPresentModeKHR& presentMode) {

        VkSurfaceCapabilitiesKHR capabilities;
        VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities));

        const VkSwapchainCreateInfoKHR swapchainInfo{
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .surface = surface,
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
        };

        VK_CHECK(vkCreateSwapchainKHR(device, &swapchainInfo, allocator, &swapchain.handle));
    }

    void Renderer::createImageView(VkSurfaceFormatKHR& surfaceFormat) {
        VK_CHECK(vkGetSwapchainImagesKHR(device, swapchain.handle, &swapchain.imageCount, nullptr));
        swapchain.images.resize(swapchain.imageCount * sizeof(VkImage));
        VK_CHECK(vkGetSwapchainImagesKHR(device, swapchain.handle, &swapchain.imageCount, swapchain.images.data()));
        swapchain.imageViews.resize(swapchain.imageCount * sizeof(VkImageView));

        for (uint32_t i = 0; i < swapchain.imageCount; ++i) {
            const VkImageViewCreateInfo imageViewInfo{
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .image = swapchain.images[i],
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
            VK_CHECK(vkCreateImageView(device, &imageViewInfo, allocator, &swapchain.imageViews[i]));
        }
    }

    void Renderer::createGraphicsPipeline() {

        const VkGraphicsPipelineCreateInfo graphicsPipelineInfo{
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stageCount = 0,
            .pStages = nullptr,
            .pVertexInputState = nullptr,
            .pInputAssemblyState = nullptr,
            .pTessellationState = nullptr,
            .pViewportState = nullptr,
            .pRasterizationState = nullptr,
            .pMultisampleState = nullptr,
            .pDepthStencilState = nullptr,
            .pColorBlendState = nullptr,
            .pDynamicState = nullptr,
            .layout = 0,
            .renderPass = 0,
            .subpass = 0,
            .basePipelineHandle = 0,
            .basePipelineIndex = 0,
        };
        vkCreateGraphicsPipelines(device, nullptr, 1, &graphicsPipelineInfo, allocator, &graphicsPipeline);
    }

} // namespace coral