#include "buffer.hpp"

#include "../pch.hpp"
#include "../utilities.hpp"
#include "core/commandBuffer.hpp"
#include "core/queue.hpp"
#include "core/synchronize.hpp"
namespace
{
   namespace vkm = my_library::vkm;

   uint32_t
   findMemoryType( const vkm::device::MemoryProperties&    mem_properties,
                   const uint32_t&                         filter,
                   const vkm::device::MemoryPropertyFlags& properties )
   {
      for ( uint32_t i { 0 }; i < mem_properties.memoryTypeCount; ++i )
      {
         // メモリタイプをチェック。
         // その後、必要なプロパティがすべて備わったものを探す必要がある
         if ( ( filter & ( 1 << i ) ) && ( mem_properties.memoryTypes[ i ].propertyFlags & properties ) == properties )
            return i;
      }
      my_library::utl::runtimeError( "failed to find suitable memory type!" );
      return {};
   }
}    // namespace
namespace my_library::vkm
{
   void
   Buffer::create( const size_t& size, const buffer::UsageFlags& usage, const queue::SharingMode& sharing_mode )
   {
      buffer::CreateInfo info( {}, size, usage, sharing_mode );
      _buffer = _device->createBufferUnique( info, nullptr, _dld );
      utl::runtimeError( _buffer->operator bool(), "failed to create buffer!" );
      utl::log( "create buffer succeeded." );
      // 成功したらサイズを保持しておく
      _buffer_size = static_cast<device::Size>( size );
   }
   void
   Buffer::allocateMemory( const device::MemoryProperties&    mem_properties,
                           const device::MemoryPropertyFlags& properties )
   {
      device::MemoryRequirements mem_requiremets { _device->getBufferMemoryRequirements( _buffer.get(), _dld ) };
      device::MemoryAllocateInfo alloc_info(
        mem_requiremets.size, findMemoryType( mem_properties, mem_requiremets.memoryTypeBits, properties ) );
      _memory = _device->allocateMemoryUnique( alloc_info, nullptr, _dld );
      utl::runtimeError( _buffer->operator bool(), "failed to allocate memory!" );
      utl::log( "allocate buffer memory succeeded." );

      _device->bindBufferMemory( _buffer.get(), _memory.get(), 0, _dld );
   }
   void
   Buffer::fillingMemory( const void* src )
   {
      void* data { _device->mapMemory( _memory.get(), 0, _buffer_size, {}, _dld ) };
      memcpy( data, src, static_cast<size_t>( _buffer_size ) );
      _device->unmapMemory( _memory.get(), _dld );
   }
   void
   Buffer::copyBuffer( const buffer::UniqueHandle& srcbuffer, const device::Size& size )
   {
      CommandBuffers command_buffer { _device, _dld };
      command_buffer.createPool();
      command_buffer.bufferAllcate( 1u );    // 1つだけ
      Command                  command { command_buffer.buffers[ 0 ], _dld };
      command::BufferBeginInfo begin_info { command::BufferUsageFlagBits::eOneTimeSubmit, 0, nullptr };
      command.beginCommandBuffer( begin_info );

      assert( this->_buffer_size >= size );
      buffer::CopyRegion region { 0, 0, size };

      command.copyBuffer( srcbuffer, this->vk_buffer, std::vector { region } );
      command.endCommandBuffer();
      GraphicsQueue g_queue { _device, _dld };
      g_queue.submit( {}, {}, command_buffer.buffers[0] );
      g_queue.waitIdle();
   }
   Buffer::Buffer( const device::UniqueHandle& device, const DispatchLoaderDynamic& dld )
     : _device { device }, _dld { dld }, _buffer {}, vk_buffer { _buffer }
   {}
   Buffer::~Buffer() {}
}    // namespace my_library::vkm
