#pragma once
#include "vkObjectTypes.hpp"
namespace my_library::vkm::ext
{
   const bool
   checkExtensionSupport( const std::vector<const char*>&               extensions,
                            const my_library::vkm::DispatchLoaderDynamic& dld );
   const bool
   check_device_ext_support( const PhysicalDevice& device );

   const std::vector<const char*>
   get_device_exts();
}    // namespace my_library::vulkan::ext
