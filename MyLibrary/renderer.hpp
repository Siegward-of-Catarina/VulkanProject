#pragma once
#include <glm/vec4.hpp>
namespace my_library
{
   namespace window
   {
      class GLwindow;
   }
   namespace renderer
   {
      enum Platform
      {
         DirectX,
         Vulkan
      };
   }
   class Graphics;
   class Renderer
   {
   public:
      Renderer( const std::unique_ptr<window::GLwindow>& gl_window, const std::string& app_name, const renderer::Platform& platform);
      ~Renderer();
      void
      init();
      void
      beginPass();
      void
      clearTarget( const float& r, const float& g, const float& b, const float& a );
      void
      clearTarget( glm::vec4 color = glm::vec4( .0f, .0f, .0f, 1.0f ) );
      void
      clearTarget( const std::array<float, 4>& color );
      void
      draw();
      void
      endPass();
      void
      present();
      void
      waitIdle();
   private:
      std::unique_ptr<Graphics> _graphics;
   };
}