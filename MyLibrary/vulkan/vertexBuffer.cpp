#include "vertexBuffer.hpp"

#include "../pch.hpp"
#include "../shader/basicShader.hpp"
#include "../utilities.hpp"
#include "../vertex.hpp"
#include "Buffer.hpp"

namespace my_library::vkm
{
   void
   VertexBuffer::create( const std::vector<vertex::BasicLayout>& vertices,
                         const device::MemoryProperties&         mem_properties )
   {
      const device::Size size = static_cast<device::Size>( ( sizeof( vertices ) * vertices.size() ) );

      Buffer staging { _device, _dld };
      staging.create( size, buffer::UsageFlagBits::eTransferSrc, queue::SharingMode::eExclusive );
      staging.allocateMemory(
        mem_properties, device::MemoryPropertyFlagBits::eHostVisible | device::MemoryPropertyFlagBits::eHostCoherent );
      staging.fillingMemory( vertices.data() );

      _buffer->create( size,
                       buffer::UsageFlagBits::eTransferDst | buffer::UsageFlagBits::eVertexBuffer,
                       queue::SharingMode::eExclusive );
      _buffer->allocateMemory( mem_properties, device::MemoryPropertyFlagBits::eDeviceLocal );

      _buffer->copyBuffer( staging.vk_buffer, size );
   }
   void
   VertexBuffer::copyBuffer( const std::vector<vertex::BasicLayout>& vertices )
   {
      _buffer->fillingMemory( vertices.data() );
   }
   const buffer::UniqueHandle&
   VertexBuffer::vk_buffer()
   {
      return _buffer->vk_buffer;
   }
   VertexBuffer::VertexBuffer( const device::UniqueHandle& device, const DispatchLoaderDynamic& dld )
     : _device { device }, _dld { dld }, _buffer { std::make_unique<Buffer>( device, dld ) }
   {}
   VertexBuffer::~VertexBuffer() {}
}    // namespace my_library::vkm
