#pragma once

#include <memory>

namespace my_library
{
    namespace window
    {
       class GLwindow;
   }
   namespace vkm
   {
      class Vulkan;
   }

   class Renderer
   {
   public:
      void
      init( std::unique_ptr<window::GLwindow>& gl_window, const bool debug = false );

      Renderer();
      ~Renderer();

   private:
      std::unique_ptr<vkm::Vulkan> _vulkan;
   };
}    // namespace my_library
