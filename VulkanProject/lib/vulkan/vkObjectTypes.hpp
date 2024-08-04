#pragma once

#include <vulkan/vulkan.hpp>

namespace my_library::vkm
{
   using DispatchLoaderDynamic = vk::DispatchLoaderDynamic;
   template<typename... ChainElements>
   using StructureChain = vk::StructureChain<ChainElements...>;
   using ApplicationInfo = vk::ApplicationInfo;
   //----instance------------------------------------
   using InstanceCreateInfo = vk::InstanceCreateInfo;
   using UniqueInstance = vk::UniqueHandle<vk::Instance, DispatchLoaderDynamic>;
   //----extension-----------------------------------
   using ExtensionProperties = vk::ExtensionProperties;
   //----debug utile---------------------------------
   namespace debug_utl
   {
      using MesseagSeverityFlagsEXT    = vk::DebugUtilsMessageSeverityFlagsEXT;
      using MessageSeverityFlagBitsEXT = vk::DebugUtilsMessageSeverityFlagBitsEXT;

      using MessageTypeFlagsEXT    = vk::DebugUtilsMessageTypeFlagsEXT;
      using MessageTypeFlagBitsEXT = vk::DebugUtilsMessageTypeFlagBitsEXT;

      using MessengerCallbackDataEXT = vk::DebugUtilsMessengerCallbackDataEXT;
      using MessengerCreateInfoEXT   = vk::DebugUtilsMessengerCreateInfoEXT;
      using UniqueDebugUtilsMessengerEXT       = vk::UniqueHandle<vk::DebugUtilsMessengerEXT, DispatchLoaderDynamic>;
   }    // namespace debug_utl
   //----------------------------------------
   using PhysicalDevice          = vk::PhysicalDevice;
   using PhysicalDeviceFeatures = vk::PhysicalDeviceFeatures;
   //----------------------------------------
   using QueueCreateInfo  = vk::DeviceQueueCreateInfo;
   using DeviceCreateInfo = vk::DeviceCreateInfo;
   using UniqueDevice        = vk::UniqueHandle<vk::Device, DispatchLoaderDynamic>;
   //-----------------------------------------
   using Queue = vk::Queue;
   //-----------------------------------------
   using UniqueSurface = vk::UniqueSurfaceKHR;
   //----swapchain----------------------------
   using SurfaceCapabilitiesKHR = vk::SurfaceCapabilitiesKHR;
   using SurfaceFormatKHR = vk::SurfaceFormatKHR;
   using PresentModeKHR = vk::PresentModeKHR;
}    // namespace my_library::vulkan
