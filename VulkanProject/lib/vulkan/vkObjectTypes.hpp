#pragma once

#include <vulkan/vulkan.hpp>

namespace my_library::vkm
{
   using DispatchLoaderDynamic = vk::DispatchLoaderDynamic;
   template<typename... ChainElements>
   using StructureChain  = vk::StructureChain<ChainElements...>;
   using ApplicationInfo = vk::ApplicationInfo;
   using LayerProperties = vk::LayerProperties;
   //----instance------------------------------------
   using InstanceCreateInfo = vk::InstanceCreateInfo;
   using UniqueInstance     = vk::UniqueHandle<vk::Instance, DispatchLoaderDynamic>;
   //----extension-----------------------------------
   using ExtensionProperties = vk::ExtensionProperties;
   //----debug utile---------------------------------
   using DebugUtilsMesseagSeverityFlagsEXT    = vk::DebugUtilsMessageSeverityFlagsEXT;
   using DebugUtilsMessageSeverityFlagBitsEXT = vk::DebugUtilsMessageSeverityFlagBitsEXT;

   using DebugUtilsMessageTypeFlagsEXT    = vk::DebugUtilsMessageTypeFlagsEXT;
   using DebugUtilsMessageTypeFlagBitsEXT = vk::DebugUtilsMessageTypeFlagBitsEXT;

   using DebugUtilsMessengerCallbackDataEXT = vk::DebugUtilsMessengerCallbackDataEXT;
   using DebugUtilsMessengerCreateInfoEXT   = vk::DebugUtilsMessengerCreateInfoEXT;
   using UniqueDebugUtilsMessengerEXT       = vk::UniqueHandle<vk::DebugUtilsMessengerEXT, DispatchLoaderDynamic>;
   //----------------------------------------
   using PhysicalDevice         = vk::PhysicalDevice;
   using PhysicalDeviceFeatures = vk::PhysicalDeviceFeatures;
   //----------------------------------------
   using QueueCreateInfo  = vk::DeviceQueueCreateInfo;
   using DeviceCreateInfo = vk::DeviceCreateInfo;
   using UniqueDevice     = vk::UniqueHandle<vk::Device, DispatchLoaderDynamic>;
   //-----------------------------------------
   using Queue = vk::Queue;
   //-----------------------------------------
   using UniqueSurface = vk::UniqueSurfaceKHR;
   //----swapchain----------------------------
   using SurfaceCapabilitiesKHR = vk::SurfaceCapabilitiesKHR;
   using SurfaceFormatKHR       = vk::SurfaceFormatKHR;
   using Format                 = vk::Format;
   using ColorSpace             = vk::ColorSpaceKHR;
   using PresentModeKHR         = vk::PresentModeKHR;
   using SwapChainCreateInfo    = vk::SwapchainCreateInfoKHR;
   using ImageUsageFlagBits     = vk::ImageUsageFlagBits;
   using ImageUsageFlags        = vk::ImageUsageFlags;
   using SharingMode            = vk::SharingMode;
   using CompositeAlphaFlagBits = vk::CompositeAlphaFlagBitsKHR;
   using CompositeAlphaFlags    = vk::CompositeAlphaFlagsKHR;
}    // namespace my_library::vkm
