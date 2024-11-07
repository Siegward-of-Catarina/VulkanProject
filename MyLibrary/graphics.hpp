#pragma once

#include "./enumExtensions.hpp"
#include "./utilities.hpp"
#include "./vulkan/vkObjectTypes.hpp"

namespace my_library
{
   namespace window
   {
      class GLwindow;
   }
   namespace vkm
   {
      struct vkDevice
      {
         const device::UniqueHandle&  device;
         const DispatchLoaderDynamic& dld;
         vkDevice();
         explicit vkDevice( const device::UniqueHandle& device_, const DispatchLoaderDynamic& dld_ );
         ~vkDevice();
      };
   }    // namespace vkm

   class Graphics
   {
   public:
      virtual void
      init() = 0;
      virtual void
      beginPass() = 0;
      virtual void
      clearTarget( const float& r, const float& g, const float& b, const float& a ) = 0;
      virtual void
      draw() = 0;
      virtual void
      endPass() = 0;
      virtual void
      present() = 0;
      virtual void
      waitIdle() = 0;

      virtual const vkm::vkDevice
      getDevice();

      Graphics();
      virtual ~Graphics();
   };
}    // namespace my_library
