#pragma once

#include <iostream>
#include <memory>

struct GLFWwindow;
struct GLFWmonitor;
namespace my_library::window
{
    struct gl_deleter
    {
       void
       operator()( GLFWwindow* ptr );
    };

   class glwindow
   {
   public:
      void
      init();
      int
      close();
      void
      pool_event();
      GLFWwindow*
      raw_glfwW_ptr();
      const std::uint32_t
      width();
      const std::uint32_t
      height();

      explicit glwindow( const std::uint32_t width, const std::uint32_t height, const char* title );
      glwindow() = delete;
      ~glwindow();

   private:

      std::unique_ptr<GLFWwindow, gl_deleter> _glfw_window;

      const std::uint32_t _width;
      const std::uint32_t _height;
      const char*         _title;
   };

}    // namespace my_library::window
