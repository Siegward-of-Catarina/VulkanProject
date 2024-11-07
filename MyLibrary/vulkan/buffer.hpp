#pragma once
#include "vkObjectTypes.hpp"
namespace my_library
{
   namespace vkm
   {
      class Buffer
      {
      public:
         void
         create( const size_t& size, const buffer::UsageFlags& usage, const queue::SharingMode& sharing_mode );
         void
         allocateMemory( const device::MemoryProperties&    mem_properties,
                         const device::MemoryPropertyFlags& properties );
         void
         fillingMemory( const void* src );
         void
         copyBuffer( const buffer::UniqueHandle& srcbuffer, const device::Size& size );
         Buffer( const device::UniqueHandle& device, const DispatchLoaderDynamic& dld );
         ~Buffer();

      public:
         const buffer::UniqueHandle& vk_buffer;

      private:
         const device::UniqueHandle&  _device;
         const DispatchLoaderDynamic& _dld;

         buffer::UniqueHandle _buffer;
         device::UniqueMemory _memory;
         device::Size         _buffer_size;
      };
   }    // namespace vkm
}    // namespace my_library
