#pragma once

#include <vulkan/vulkan.hpp>

namespace my_library::vulkan
{
   using unq_vk_instance               = vk::UniqueHandle<vk::Instance, vk::DispatchLoaderDynamic>;
   using unq_vk_debugutils_messenger_ext = vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic>;
   using vk_physicaldevice = vk::PhysicalDevice;
}    // namespace my_library::vulkan