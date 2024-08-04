#pragma once

#include <iostream>
#include <memory>

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
   public:
      void
      init();
      int
      close();
      void
      poolEvent();
      GLFWwindow*
      rawGLFWwindowPtr();
      const std::uint32_t&
      width();
      const std::uint32_t&
      height();
      const std::string&
      title();
      //‚Ì‚¿‚É•ª—£‚·‚é—\’è
      const std::vector<const char*>
      getRequiredInstanceExtensions( const bool debug ) const;

      explicit GLwindow( const std::uint32_t& width, const std::uint32_t& height, const std::string& title );
      GLwindow() = delete;
      ~GLwindow();

   private:
      std::unique_ptr<GLFWwindow, GLdeleter> _glfw_window;

      const std::uint32_t _width;
      const std::uint32_t _height;
      const std::string   _title;
   };

}    // namespace my_library::window
