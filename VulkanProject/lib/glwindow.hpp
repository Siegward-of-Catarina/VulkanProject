#pragma once
#include "../api/window.hpp"

#include <iostream>
struct GLFWwindow;
struct GLFWmonitor;
namespace lib::window
{
   class glwindow : public api::window::glwindow
   {
   private:
      glwindow() = delete;
      ~glwindow();

   private:
      GLFWwindow*         glfw_window;
      const std::uint32_t width;
      const std::uint32_t height;
      const char*         title;

   public:
      explicit glwindow( const std::uint32_t width, const std::uint32_t height, const char* title );
      void
      init();
      int
      close();
      void
      pool_event();
      void
      release();

      const GLFWwindow*
      get_window();
      const std::uint32_t
      get_width();
      const std::uint32_t
      get_height();
   };

}    // namespace lib::window
