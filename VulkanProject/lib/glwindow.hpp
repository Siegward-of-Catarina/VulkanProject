#pragma once
#include "create_helper.hpp"

#include <iostream>
struct GLFWwindow;
struct GLFWmonitor;
namespace my_library::window
{

   class glwindow
   {
   private:
      explicit glwindow( const std::uint32_t width, const std::uint32_t height, const char* title );
      glwindow() = delete;
      ~glwindow();

      CREATE_HELPER;
      using create_helper = Impl<glwindow>;

   private:
      GLFWwindow*         glfw_window;
      const std::uint32_t width;
      const std::uint32_t height;
      const char*         title;

   public:
      static glwindow*
      create( const std::uint32_t width, const std::uint32_t height, const char* title );

   public:
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

}    // namespace my_library::window
