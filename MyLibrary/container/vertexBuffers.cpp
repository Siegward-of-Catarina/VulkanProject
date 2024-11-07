#include "../pch.hpp"
#include "vertexBuffers.hpp"
#include "../vertex.hpp"
#include "../vulkan/buffer.hpp"
#include "../vulkan/vertexBuffer.hpp"
namespace my_library::ctr
{
   VertexBuffers::VertexBuffers( const vkm::device::UniqueHandle& device, const vkm::DispatchLoaderDynamic& dld )
     : _device { device }, _dld { dld },_buffers{}
   {}
   VertexBuffers::~VertexBuffers() {}
   void
   VertexBuffers::addVertexBuffer()
   {}
   void
   VertexBuffers::removeVertexBuffer()
   {}
}    // namespace my_library::ctr