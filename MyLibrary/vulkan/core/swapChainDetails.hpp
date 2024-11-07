#pragma once 
#include "../vkObjectTypes.hpp"
#include<glm/vec2.hpp>
#include<optional>
namespace my_library::vkm
{
   class SwapChainDetails
   {
   public:
      const bool
      adequate();
      surface::Format
      chooseSwapSurfaceFormat( const image::Format&     format      = image::Format::eB8G8R8A8Srgb,
                               const image::ColorSpace& color_space = image::ColorSpace::eSrgbNonlinear ) const;
      swapchain::PresentMode
      chooseSwapPresentMode( const std::optional<swapchain::PresentMode>& present_mode ) const;
      image::Extent2D
      chooseSwapExtent( const glm::u32vec2& glfw_fbuffer_size ) const;
      explicit SwapChainDetails( const device::Gpu& device, const surface::UniqueHandle& surface );
      SwapChainDetails();
      ~SwapChainDetails();
      SwapChainDetails&
      operator=( const SwapChainDetails& s );

   public:
      const surface::Capabilities&        capabilities;
      const std::vector<surface::Format>& formats;
      const std::vector<swapchain::PresentMode>&   present_modes;

   private:
      surface::Capabilities               _capabilities;
      std::vector<surface::Format>        _formats;
      std::vector<swapchain::PresentMode> _present_modes;
   };
}
