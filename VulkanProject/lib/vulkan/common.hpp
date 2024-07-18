#pragma once

#include <vulkan/vulkan.hpp>

namespace my_library::vulkan
{
   using UniqueInstance               = vk::UniqueHandle<vk::Instance, vk::DispatchLoaderDynamic>;
   using UniqueDebugUtilsMessengerEXT = vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic>;
}    // namespace my_library::vulkan