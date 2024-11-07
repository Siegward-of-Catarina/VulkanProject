#pragma once
#include "../vkObjectTypes.hpp"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
namespace my_library::vkm
{
   class Command
   {
   public:
      void
      resetCommandBuffer();
      void
      beginCommandBuffer( const command::BufferBeginInfo& info = { {}, 0, nullptr } );
      void
      beginRenderPass( render_pass::BeginInfo& renderpass_info, const render_pass::SubpassContents& contents );
      void
      beginRenderPass( const render_pass::UniqueHandle&    render_pass,
                       const framebuffer::UniqueHandle&    buffer,
                       const image::Offset2D&              offset,
                       const image::Extent2D&              extent,
                       const glm::vec4&                    color,
                       const render_pass::SubpassContents& contents );
      void
      bindPipeline( const pipeline::UniqueHandle& pipeline, const pipeline::BindPoint& bind_point );
      void
      bindVertexBuffer( const uint32_t&                      first_binding,
                        const std::vector<buffer::vkBuffer>& buffers,
                        const std::vector<device::Size>&     offsets );
      void
      setViewport( const framebuffer::Viewport& viewport );
      void
      setViewport( const glm::vec2& position, const glm::vec2& extent, const glm::vec2& depth_min_max );
      void
      setScissor( const framebuffer::Rect2D& scissor );
      void
      setScissor( const image::Offset2D& offset, const image::Extent2D& extent );
      void
      setPolygonMode( const pipeline::PolygonMode& mode );
      void
      copyBuffer( const buffer::UniqueHandle& src, const buffer::UniqueHandle& dst, const std::vector<buffer::CopyRegion>& copy_regions);
      void
      draw( const uint32_t& vertex_count );
      void
      endRenderPass();
      void
      endCommandBuffer();
      Command( const command::UniqueBuffer& command_buffer, const DispatchLoaderDynamic& dld );
      ~Command();

   private:
      const command::UniqueBuffer& _command_buffer;
      const DispatchLoaderDynamic& _dld;
   };
   class CommandBuffers
   {
   public:
      void
      createPool();
      void
      bufferAllcate( const uint32_t& max_frame );
      CommandBuffers( const device::UniqueHandle& device, const DispatchLoaderDynamic& dld );
      ~CommandBuffers();

   public:
      const std::vector<command::UniqueBuffer>& buffers;

   private:
      const device::UniqueHandle&  _device;
      const DispatchLoaderDynamic& _dld;

      command::UniquePool                _command_pool;
      std::vector<command::UniqueBuffer> _command_buffers;
   };
}    // namespace my_library::vkm
