#pragma once
#include "../graphics.hpp"
namespace my_library
{
   namespace ctr
   {
      class VertexBuffers;
   }
   namespace window
   {
      class GLwindow;
      class GLwindowForVk;
   }    // namespace window
   namespace vkm
   {
      class Instance;
      class DebugMessenger;
      class Device;
      class Surface;
      class SwapChain;
      class Pipeline;
      class CommandBuffers;
      class Synchronize;
      class VertexBuffer;

      class VkGraphics : public Graphics
      {
      public:
         VkGraphics( const std::unique_ptr<window::GLwindow>& gl_window, const std::string& app_name );
         ~VkGraphics() override;
         void
         init() override;
         void
         beginPass() override;
         void
         clearTarget( const float& r, const float& g, const float& b, const float& a ) override;
         void
         draw() override;
         void
         endPass() override;
         void
         present() override;
         void
         waitIdle() override;
         const vkDevice
         getDevice() override;

         const std::unique_ptr<ctr::VertexBuffers>
         createVertexBuffers();

      private:
         void
         recreateSwapChain();

      private:
         const std::string                      _app_name;
         std::unique_ptr<Instance>              _instance;
         std::unique_ptr<DebugMessenger>        _dbg_messenger;
         std::unique_ptr<Device>                _device;

         std::unique_ptr<window::GLwindowForVk> _window;
         std::unique_ptr<Surface>               _surface;
         std::unique_ptr<SwapChain>             _swapchain;

         std::unique_ptr<Pipeline>              _pipeline;
         std::unique_ptr<CommandBuffers>        _command_buffers;
         std::unique_ptr<Synchronize>           _sync;
         std::unique_ptr<VertexBuffer>          _vertexbuffer;
      };
   }    // namespace vkm
}    // namespace my_library
