#pragma once

#include <vulkan/vulkan.hpp>

namespace my_library::vulkan
{
   using vk_dispatchloader_dynamic = vk::DispatchLoaderDynamic;
   //----------------------------------------
   using unq_vk_instance = vk::UniqueHandle<vk::Instance, vk_dispatchloader_dynamic>;
   //----------------------------------------
   using vk_debugutils_messenger_createinfo_ext = vk::DebugUtilsMessengerCreateInfoEXT;
   using unq_vk_debugutils_messenger_ext = vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk_dispatchloader_dynamic>;
   //----------------------------------------
   using vk_physicaldevice          = vk::PhysicalDevice;
   using vk_physicaldevice_features = vk::PhysicalDeviceFeatures;
   //----------------------------------------
   using vk_queue_createinfo  = vk::DeviceQueueCreateInfo;
   using vk_device_createinfo = vk::DeviceCreateInfo;
   using unq_vk_device        = vk::UniqueHandle<vk::Device, vk_dispatchloader_dynamic>;
   //-----------------------------------------
   using vk_queue = vk::Queue;
   //-----------------------------------------
   using unq_vk_surface = vk::UniqueSurfaceKHR; //vk::UniqueHandle<vk::SurfaceKHR, vk_dispatchloader_dynamic>;
}    // namespace my_library::vulkan
