#include "commandBuffer.hpp"

#include "../../pch.hpp"
#include "../../utilities.hpp"
#include "queuefamily.hpp"

namespace my_library::vkm
{
   void
   Command::resetCommandBuffer()
   {
      _command_buffer->reset( {}, _dld );
   }
   void
   Command::beginCommandBuffer( const command::BufferBeginInfo& info )
   {
      _command_buffer->begin( info, _dld );
   }
   void
   Command::beginRenderPass( render_pass::BeginInfo& renderpass_info, const render_pass::SubpassContents& contents )
   {
      _command_buffer->beginRenderPass( renderpass_info, contents, _dld );
   }
   void
   Command::beginRenderPass( const render_pass::UniqueHandle&    render_pass,
                             const framebuffer::UniqueHandle&    buffer,
                             const image::Offset2D&              offset,
                             const image::Extent2D&              extent,
                             const glm::vec4&                    color,
                             const render_pass::SubpassContents& contents )
   {
      framebuffer::ClearValue color_v {
         std::array {color.r, color.g, color.b, color.a}
      };
      framebuffer::Rect2D render_area {
         {0, 0},
         extent
      };
      render_pass::BeginInfo info( render_pass.get(), buffer.get(), render_area, color_v );
      this->beginRenderPass( info, contents );
   }
   void
   Command::bindPipeline( const pipeline::UniqueHandle& pipeline, const pipeline::BindPoint& bind_point )
   {
      _command_buffer->bindPipeline( bind_point, pipeline.get(), _dld );
   }
   void
   Command::bindVertexBuffer( const uint32_t&                      first_binding,
                              const std::vector<buffer::vkBuffer>& buffers,
                              const std::vector<device::Size>&     offsets )
   {
      _command_buffer->bindVertexBuffers( first_binding, buffers, offsets, _dld );
   }
   void
   Command::setViewport( const framebuffer::Viewport& viewport )
   {
      _command_buffer->setViewport( 0, viewport, _dld );
   }
   void
   Command::setViewport( const glm::vec2& position, const glm::vec2& extent, const glm::vec2& depth_min_max )
   {
      framebuffer::Viewport view( position.x, position.y, extent.x, extent.y, depth_min_max.x, depth_min_max.y );
      this->setViewport( view );
   }
   void
   Command::setScissor( const framebuffer::Rect2D& scissor )
   {
      _command_buffer->setScissor( 0, scissor, _dld );
   }
   void
   Command::setScissor( const image::Offset2D& offset, const image::Extent2D& extent )
   {
      this->setScissor( framebuffer::Rect2D( offset, extent ) );
   }
   void
   Command::setPolygonMode( const pipeline::PolygonMode& mode )
   {
      _command_buffer->setPolygonModeEXT( mode, _dld );
   }
   void
   Command::copyBuffer( const buffer::UniqueHandle&            src,
                        const buffer::UniqueHandle&            dst,
                        const std::vector<buffer::CopyRegion>& copy_regions )
   {
      _command_buffer->copyBuffer( src.get(), dst.get(), copy_regions, _dld );
   }
   void
   Command::draw( const uint32_t& vertex_count )
   {
      _command_buffer->draw( vertex_count, 1, 0, 0, _dld );
   }
   void
   Command::endRenderPass()
   {
      _command_buffer->endRenderPass( _dld );
   }
   void
   Command::endCommandBuffer()
   {
      _command_buffer->end( _dld );
   }
   Command::Command( const command::UniqueBuffer& command_buffer, const DispatchLoaderDynamic& dld )
     : _command_buffer { command_buffer }, _dld { dld }
   {}
   Command::~Command() {}
}    // namespace my_library::vkm
namespace my_library::vkm
{
   CommandBuffers::CommandBuffers( const device::UniqueHandle& device, const DispatchLoaderDynamic& dld )
     : _device { device }, _dld { dld }, _command_pool {}, _command_buffers {}, buffers { _command_buffers }
   {}
   CommandBuffers::~CommandBuffers() {}
   void
   CommandBuffers::createPool()
   {
      command::PoolCreateInfo info( command::PoolCreateFlagBits::eResetCommandBuffer, Queuefamily::graphicsfamily() );
      _command_pool = _device->createCommandPoolUnique( info, nullptr, _dld );
      utl::runtimeError( _command_pool->operator bool(), "faild to create command pool!" );
      utl::log( "create command pool succeeded." );
   }
   void
   CommandBuffers::bufferAllcate( const uint32_t& max_frame )
   {
      command::BufferAllocateInfo info( _command_pool.get(), command::BufferLevel::ePrimary, max_frame );
      _command_buffers = _device->allocateCommandBuffersUnique( info, _dld );
      utl::runtimeError( !_command_buffers.empty(), "faild to allcate command buffers!" );
      utl::log( "allcate command buffers succeeded." );
   }
}    // namespace my_library::vkm
