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

   const vk_surface_format_khr
   details::choose_swap_surfaceformat( const vk::Format&        format,
                                       const vk::ColorSpaceKHR& colorspace )
   {
      assert( !formats.empty() );
      for ( const auto& availableformat : formats )
      {
         if ( availableformat.format == format && availableformat.colorSpace == colorspace ) { return availableformat; }
      }
      return formats[ 0 ];
   }

   const vk_present_mode_khr
   details::choose_swap_presentmode( const vk::PresentModeKHR& presentmode )
   {
      assert( !present_modes.empty() );
      for ( const auto& available_mode : present_modes )
      {
         if ( available_mode == presentmode ) return available_mode;
      }

      return vk::PresentModeKHR::eFifo;
   }

}    // namespace my_library::vulkan::swapchain_support
