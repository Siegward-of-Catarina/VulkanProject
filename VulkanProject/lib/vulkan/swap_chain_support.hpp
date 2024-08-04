#pragma once
#include "vkObjectTypes.hpp"

#include <vector>
namespace my_library::vkm::swapchain_support
{
   struct details
   {
      SurfaceCapabilitiesKHR        capabilities;
      std::vector<SurfaceFormatKHR> formats;
      std::vector<PresentModeKHR>   present_modes;

      const bool
      adequate();
   };
   const details
   query_details( const PhysicalDevice& device, const UniqueSurface& surface );
}    // namespace my_library::vulkan::swap_chain_support
