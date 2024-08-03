#pragma once
#include "vk_object_types.hpp"

#include <vector>
namespace my_library::vulkan::swapchain_support
{
   struct details
   {
      vk_surface_capabilities_khr        capabilities;
      std::vector<vk_surface_format_khr> formats;
      std::vector<vk_present_mode_khr>   present_modes;

      const bool
      adequate();
   };
   const details
   query_details( const vk_physicaldevice& device, const unq_vk_surface& surface );
}    // namespace my_library::vulkan::swap_chain_support
