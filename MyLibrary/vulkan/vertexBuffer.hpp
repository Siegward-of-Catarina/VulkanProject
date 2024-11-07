#pragma once
#include "vkObjectTypes.hpp"

namespace my_library
{
   namespace vertex
   {
      struct BasicLayout;
   }
   namespace vkm
   {
      class Buffer;

      class VertexBuffer
      {
      public:
         void
         create( const std::vector<vertex::BasicLayout>& vertices, const device::MemoryProperties& mem_properties );
         void
         copyBuffer( const std::vector<vertex::BasicLayout>& vertices );
         const buffer::UniqueHandle&
         vk_buffer();
         VertexBuffer( const device::UniqueHandle& device, const DispatchLoaderDynamic& dld );
         ~VertexBuffer();

      private:
         const device::UniqueHandle&  _device;
         const DispatchLoaderDynamic& _dld;
         std::unique_ptr<Buffer>      _buffer;
      };
   }    // namespace vkm
}    // namespace my_library
