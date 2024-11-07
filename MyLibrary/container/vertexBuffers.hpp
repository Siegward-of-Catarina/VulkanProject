#pragma once
#include "../utilities.hpp"
#include "../vulkan/vkObjectTypes.hpp"

#include <concepts>
template<class T>
concept Vertex = requires( const T& t ) { t.layout; };

namespace my_library
{
   namespace vkm
   {
      class VertexBuffer;
   }
   namespace ctr
   {
      class VertexBuffers
      {
      public:
         VertexBuffers(const vkm::device::UniqueHandle& device, const vkm::DispatchLoaderDynamic& dld);
         ~VertexBuffers();
         void
         addVertexBuffer();
         void
         removeVertexBuffer();
      private:
         const vkm::device::UniqueHandle&  _device;
         const vkm::DispatchLoaderDynamic& _dld;
         std::vector<vkm::VertexBuffer>    _buffers;
      };
   }    // namespace ctr
}    // namespace my_library
