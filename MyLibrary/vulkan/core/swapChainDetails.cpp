#include "swapChainDetails.hpp"

#include "../../pch.hpp"
namespace my_library::vkm
{
   const bool
   SwapChainDetails::adequate()
   {
      return !this->_formats.empty() && !this->_present_modes.empty();
   }
   surface::Format
   SwapChainDetails::chooseSwapSurfaceFormat( const image::Format& format, const image::ColorSpace& color_space ) const
   {
      for ( auto& available_format : _formats )
      {
         if ( available_format.format == format && available_format.colorSpace == color_space )
         {
            return available_format;
         }
      }
      return _formats[ 0 ];
   }
   swapchain::PresentMode
   SwapChainDetails::chooseSwapPresentMode( const std::optional<swapchain::PresentMode>& present_mode ) const
   {
      if ( present_mode.has_value() )
      {
         for ( auto& available_mode : _present_modes )
         {
            if ( available_mode == present_mode.value() ) { return available_mode; }
         }
      }
      return swapchain::PresentMode::eFifo;
   }
   image::Extent2D
   SwapChainDetails::chooseSwapExtent( const glm::u32vec2& glfw_fbuffer_size ) const
   {
      if ( _capabilities.currentExtent.width != ( std::numeric_limits<uint32_t>::max )() )
      {
         return _capabilities.currentExtent;
      }
      else
      {
         image::Extent2D actualExtent = { glfw_fbuffer_size.x, glfw_fbuffer_size.y };
         actualExtent.width =
           std::clamp( actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width );
         actualExtent.height =
           std::clamp( actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height );

         return actualExtent;
      }
   }
   SwapChainDetails::SwapChainDetails( const device::Gpu& device, const surface::UniqueHandle& surface )
     : _capabilities { device.getSurfaceCapabilitiesKHR( surface.get() ) }
     , _formats { device.getSurfaceFormatsKHR( surface.get() ) }
     , _present_modes { device.getSurfacePresentModesKHR( surface.get() ) }
     , capabilities { _capabilities }
     , formats { _formats }
     , present_modes { _present_modes }
   {}

   SwapChainDetails::SwapChainDetails()
     : _capabilities {}
     , _formats {}
     , _present_modes {}
     , capabilities { _capabilities }
     , formats { _formats }
     , present_modes { _present_modes }
   {}

   SwapChainDetails::~SwapChainDetails() {}

   SwapChainDetails&
   SwapChainDetails::operator=( const SwapChainDetails& s )
   {
      _capabilities  = s._capabilities;
      _formats       = s._formats;
      _present_modes = s._present_modes;
      return *this;
   }

}    // namespace my_library::vkm