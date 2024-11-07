#pragma once

#include <glm/vec2.hpp>

struct GLFWwindow;
struct GLFWmonitor;
namespace my_library::window
{
   struct GLdeleter
   {
      void
      operator()( GLFWwindow* ptr );
   };

   class GLwindow
   {
   private:
      static void
      framebufferResizeCallback( GLFWwindow* window, int width, int height );

   public:
      void
      init();
      int
      close();
      void
      poolEvent();
      void
      waitEvents();
      GLFWwindow*
      rawGLFWwindowPtr();
      const std::uint32_t&
      width();
      const std::uint32_t&
      height();
      const std::string&
      title();
      const bool
      framebufferResized();
      void
      framebufferUnResized();
      const glm::u32vec2
      getFramebufferSize();
      explicit GLwindow( const std::uint32_t& width, const std::uint32_t& height, const std::string& title );
      GLwindow() = delete;
      virtual ~GLwindow();

   private:
      std::unique_ptr<GLFWwindow, GLdeleter> _glfw_window;
      bool                                   _framebufferResized;
      std::uint32_t                          _width;
      std::uint32_t                          _height;
      std::string                            _title;
   };

}    // namespace my_library::window
