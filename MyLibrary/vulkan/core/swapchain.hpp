#pragma once
#include "../vkObjectTypes.hpp"

#include <glm/vec2.hpp>

namespace my_library
{
   namespace vkm
   {
      class SwapChain
      {
      public:
         void
         create( const device::Gpu&           device,
                 const surface::UniqueHandle& surface,
                 const glm::u32vec2&          glfw_fbuffer_size );
         void
         cleanup();
         void
         createImageViews();
         void
         createFramebuffer( const render_pass::UniqueHandle& render_pass );
         vk::Result
         updateImageIndex( const sync::UniqueSemaphore& image_available_semaphore );
         void
         updateFrameIndex( const int& max_frames_in_flight );
         SwapChain( const device::UniqueHandle& device, const DispatchLoaderDynamic& dld );
         ~SwapChain();

      public:
         const uint32_t&                current_frame_index;
         const uint32_t&                current_image_index;
         const swapchain::UniqueHandle& vk_swapchain;
         const image::Format&           format;
         const image::Extent2D&         extent;

         const std::vector<swapchain::Image>&          images;
         const std::vector<framebuffer::UniqueHandle>& framebuffers;

      private:
         const device::UniqueHandle&  _device;
         const DispatchLoaderDynamic& _dld;

         uint32_t _current_frame_index;
         uint32_t _current_image_index;

         swapchain::UniqueHandle _swapchain;
         image::Format           _format;
         image::Extent2D         _extent;

         std::vector<swapchain::Image>          _images;
         std::vector<image_view::UniqueHandle>  _imageviews;
         std::vector<framebuffer::UniqueHandle> _framebuffers;
      };
   }    // namespace vkm
}    // namespace my_library
