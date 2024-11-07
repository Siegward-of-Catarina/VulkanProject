#include "vkGraphics.hpp"

#include "../container/vertexBuffers.hpp"
#include "../pch.hpp"
#include "../utilities.hpp"
#include "../vertex.hpp"
#include "core/commandBuffer.hpp"
#include "core/debugUtils.hpp"
#include "core/device.hpp"
#include "core/instance.hpp"
#include "core/pipeline.hpp"
#include "core/queue.hpp"
#include "core/surface.hpp"
#include "core/swapchain.hpp"
#include "core/synchronize.hpp"
#include "glwindowForVk.hpp"
//
#include "vertexBuffer.hpp"
namespace
{
   namespace vkm = my_library::vkm;
   namespace utl = my_library::utl;
   constexpr int                                      MAX_FRAMES_IN_FLIGHT { 2 };
   bool                                               layerfound = false;
   const std::vector<my_library::vertex::BasicLayout> vertices   = {
      {{ 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f }},
      { { 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }},
      {{ -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }}
   };
}    // namespace
namespace
{
   bool
   checkValidationLayerSupport( const std::vector<const char*>& validationlayers )
   {
      if ( validationlayers.empty() ) return false;

      const std::vector<vk::LayerProperties> availablelayers { vk::enumerateInstanceLayerProperties() };
      bool                                   layerfound { false };
      for ( const char* layername : validationlayers )
      {
         for ( const auto& layerproperties : availablelayers )
         {
            if ( strcmp( layername, layerproperties.layerName ) == 0 )
            {
               layerfound = true;
               break;
            }
         }
         if ( !layerfound ) { return false; }
      }
      my_library::utl::log( "validationlayers is supported." );
      return true;
   }

   const std::vector<const char*>
   getValidationLayers()
   {
#ifdef NDEBUG
      const std::vector<const char*> validationlayers {};
#else
      const std::vector<const char*> validationlayers { "VK_LAYER_KHRONOS_validation" };
      // 一度だけチェックする
      static const bool valid { checkValidationLayerSupport( validationlayers ) };

      if ( !valid ) throw std::runtime_error( "validation layers requested, but not available!" );
#endif
      return validationlayers;
   }
}    // namespace

namespace my_library::vkm
{
   vkDevice::vkDevice() : device {}, dld {} {}
   vkDevice::vkDevice( const device::UniqueHandle& device_, const DispatchLoaderDynamic& dld_ )
     : device { device_ }, dld { dld_ }
   {}
   vkDevice::~vkDevice() {}
}    // namespace my_library::vkm

namespace my_library::vkm
{
   VkGraphics::VkGraphics( const std::unique_ptr<window::GLwindow>& gl_window, const std::string& app_name )
     : _window { std::make_unique<window::GLwindowForVk>( gl_window ) }
     , _app_name { app_name }
     , _instance { std::make_unique<Instance>() }
     , _dbg_messenger { std::make_unique<DebugMessenger>() }
     , _device {}
     , _surface {}
     , _swapchain {}
     , _pipeline {}
     , _command_buffers {}
     , _sync {}
   {}
   VkGraphics::~VkGraphics() {}
   void
   VkGraphics::init()
   {
      _instance->create( _app_name, _window->getRequiredInstanceExtensions(), getValidationLayers() );

      _dbg_messenger->create( _instance->vk_instance, _instance->dld );

      _surface = std::make_unique<Surface>( _window );
      _surface->create( _instance->vk_instance );

      _device = std::make_unique<Device>( _instance->enumeratePhysicalDevices(), _instance->dld );
      _device->pickUpSuitablePhysicalDevice( _surface->vk_surface );
      _device->createLogicalDevice( getValidationLayers() );

      _swapchain = std::make_unique<SwapChain>( _device->logical_device, _instance->dld );
      _swapchain->create( _device->suitable_physical_device, _surface->vk_surface, _window->getFramebufferSize() );
      _swapchain->createImageViews();

      _pipeline = std::make_unique<Pipeline>( _device->logical_device, _instance->dld );
      _pipeline->createRenderPass( _swapchain->format );
      _pipeline->createPipeline( _swapchain->extent );

      _swapchain->createFramebuffer( _pipeline->renderpass );
      _command_buffers = std::make_unique<CommandBuffers>( _device->logical_device, _instance->dld );
      _command_buffers->createPool();
      _command_buffers->bufferAllcate( MAX_FRAMES_IN_FLIGHT );

      _sync = std::make_unique<Synchronize>( _device->logical_device, _instance->dld );
      _sync->create( MAX_FRAMES_IN_FLIGHT );

      _vertexbuffer = std::make_unique<VertexBuffer>( _device->logical_device, _instance->dld );
      _vertexbuffer->create( ::vertices, _device->getGpuMemoryProperties() );
   }
   void
   VkGraphics::beginPass()
   {    // 現在のフレーム取得, 現在フレームの同期オブジェクトを取得
      auto& currentframe = _swapchain->current_frame_index;
      _sync->waitForFences( currentframe );

      Command command { _command_buffers->buffers[ currentframe ], _instance->dld };
      command.resetCommandBuffer();
      command.beginCommandBuffer();
   }
   void
   VkGraphics::clearTarget( const float& r, const float& g, const float& b, const float& a )
   {    // 現在のフレーム取得, 現在フレームの同期オブジェクトを取得
      auto& currentframe  = _swapchain->current_frame_index;
      auto  current_syncs = _sync->getCurrentObjects( currentframe );

      auto     result { _swapchain->updateImageIndex( current_syncs.image_available_semaphore ) };
      uint32_t image_index { _swapchain->current_image_index };
      // サーフェイスと互換性がなくなったら再作成
      if ( result == vk::Result::eErrorOutOfDateKHR )
      {
         recreateSwapChain();
         return;
      }

      _sync->resetFences( currentframe );

      Command command { _command_buffers->buffers[ currentframe ], _instance->dld };
      command.beginRenderPass( _pipeline->renderpass,
                               _swapchain->framebuffers[ image_index ],
                               image::Offset2D( 0, 0 ),
                               _swapchain->extent,
                               glm::vec4( r, g, b, a ),
                               render_pass::SubpassContents::eInline );
   }
   void
   VkGraphics::draw()
   {
      auto&   currentframe = _swapchain->current_frame_index;
      Command command { _command_buffers->buffers[ currentframe ], _instance->dld };
      command.bindPipeline( _pipeline->pipeline, pipeline::BindPoint::eGraphics );

      command.setViewport(
        glm::vec2( .0f, .0f ),
        glm::vec2( static_cast<float>( _swapchain->extent.width ), static_cast<float>( _swapchain->extent.height ) ),
        glm::vec2( .0f, 1.0f ) );

      command.setScissor( image::Offset2D( 0, 0 ), _swapchain->extent );

      command.setPolygonMode( pipeline::PolygonMode::eFill );

      std::vector<buffer::vkBuffer> vuffers { _vertexbuffer->vk_buffer().get() };
      std::vector<device::Size>     offset { 0 };
      command.bindVertexBuffer( 0u, vuffers, offset );

      command.draw( static_cast<uint32_t>( ::vertices.size() ) );
   }
   void
   VkGraphics::endPass()
   {
      auto&   currentframe = _swapchain->current_frame_index;
      Command command { _command_buffers->buffers[ currentframe ], _instance->dld };
      command.endRenderPass();
      command.endCommandBuffer();
   }
   void
   VkGraphics::present()
   {    // 現在のフレーム取得, 現在フレームの同期オブジェクトを取得
      auto&    currentframe  = _swapchain->current_frame_index;
      auto     current_syncs = _sync->getCurrentObjects( currentframe );
      uint32_t image_index { _swapchain->current_image_index };

      _sync->resetFences( currentframe );

      GraphicsQueue graphics_queue { _device->logical_device, _instance->dld };
      graphics_queue.submit(
        current_syncs, pipeline::StageFlagBits::eColorAttachmentOutput, _command_buffers->buffers[ currentframe ] );

      PresentQueue present_queue { _device->logical_device, _instance->dld };
      auto         result =
        present_queue.present( current_syncs.render_finished_semaphore, _swapchain->vk_swapchain, image_index );

      if ( result || _window->framebufferResized() )
      {
         _window->framebufferUnResized();
         recreateSwapChain();
         return;
      }

      _swapchain->updateFrameIndex( MAX_FRAMES_IN_FLIGHT );
   }
   void
   VkGraphics::waitIdle()
   {
      _device->logical_device->waitIdle( _instance->dld );
   }
   const vkDevice
   VkGraphics::getDevice()
   {
      return vkDevice( _device->logical_device, _instance->dld );
   }
   const std::unique_ptr<ctr::VertexBuffers>
   VkGraphics::createVertexBuffers()
   {
      return std::make_unique<ctr::VertexBuffers>( _device->logical_device, _instance->dld );
   }
   void
   VkGraphics::recreateSwapChain()
   {
      glm::u32vec2 size { _window->getFramebufferSize() };
      while ( size.x == 0 || size.y == 0 )
      {
         size = _window->getFramebufferSize();
         _window->waitEvents();
      }

      _device->logical_device->waitIdle( _instance->dld );
      _swapchain->cleanup();
      _swapchain->create( _device->suitable_physical_device, _surface->vk_surface, _window->getFramebufferSize() );
      _swapchain->createImageViews();
      _swapchain->createFramebuffer( _pipeline->renderpass );
   }
}    // namespace my_library::vkm
