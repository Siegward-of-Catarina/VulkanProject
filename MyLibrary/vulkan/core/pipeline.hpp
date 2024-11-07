#pragma once
#include "../vkObjectTypes.hpp"
namespace my_library::vkm
{
   class Pipeline
   {
   public:
      void
      createRenderPass( const image::Format& format );
      void
      createPipeline( const image::Extent2D& extent );
      Pipeline( const device::UniqueHandle& device, const DispatchLoaderDynamic& dld );
      ~Pipeline();

   public:
      const pipeline::UniqueLayout& pipelinelayout;
      const render_pass::UniqueHandle&     renderpass;
      const pipeline::UniqueHandle&       pipeline;
   private:
      const  device::UniqueHandle&          _device;
      const DispatchLoaderDynamic& _dld;

      pipeline::UniqueLayout _pipelinelayout;
      render_pass::UniqueHandle     _renderpass;
      pipeline::UniqueHandle _pipeline;
   };
}    // namespace my_library::vkm
