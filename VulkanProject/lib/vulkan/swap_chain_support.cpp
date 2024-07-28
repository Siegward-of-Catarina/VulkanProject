#include "swap_chain_support.hpp"
namespace my_library::vulkan::swapchain_support
{
   const details
   query_details( const vk_physicaldevice& device, const unq_vk_surface& surface )
   {
      details d;
      d.capabilities  = device.getSurfaceCapabilitiesKHR( surface.get() );
      d.formats       = device.getSurfaceFormatsKHR( surface.get() );
      d.present_modes = device.getSurfacePresentModesKHR( surface.get() );

      return d;
   }
   const bool
   details::adequate()
   {
      return !this->formats.empty() && !this->present_modes.empty();
   }
}    // namespace my_library::vulkan::swapchain_support
