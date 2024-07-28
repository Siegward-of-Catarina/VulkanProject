#pragma once
#include "vk_object_types.hpp"
namespace my_library::vulkan::ext
{
   const std::vector<const char*>
   get_required_extensions( const my_library::vulkan::vk_dispatchloader_dynamic& dld );

   const bool
   check_device_ext_support( const my_library::vulkan::vk_physicaldevice& device );

   const std::vector<const char*>
   get_device_exts();
}    // namespace my_library::vulkan::ext
