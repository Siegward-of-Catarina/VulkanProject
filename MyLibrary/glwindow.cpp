#include "glwindow.hpp"

#include "pch.hpp"
#include "utilities.hpp"
#define GLFW_INCLUDE_VULKAN    // GLFW には独自の定義が含まれ、それとともに Vulkan ヘッダーが自動的にロード
#include <GLFW/glfw3.h>

namespace my_library::window    // deleter
{
   void
   GLdeleter::operator()( GLFWwindow* ptr )
   {
      glfwDestroyWindow( ptr );
      glfwTerminate();
   }
}    // namespace my_library::window

namespace my_library::window    // glwindow
{
   void
   GLwindow::framebufferResizeCallback( GLFWwindow* window, int width, int height )
   {
      auto owner { reinterpret_cast<GLwindow*>( glfwGetWindowUserPointer( window ) ) };
      owner->_framebufferResized = true;
   }
   void
   GLwindow::init()
   {
      // glfw 初期化
      glfwInit();

      // OpenGLコンテキストを使用しないように指定
      glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

      // ウィンドウ初期化
      std::unique_ptr<GLFWwindow, GLdeleter> w { glfwCreateWindow(
        _width, _height, _title.c_str(), nullptr, nullptr ) };
      _glfw_window = std::move( w );
      glfwSetWindowUserPointer( _glfw_window.get(), this );
      glfwSetFramebufferSizeCallback( _glfw_window.get(), framebufferResizeCallback );
   }

   int
   GLwindow::close()
   {
      return glfwWindowShouldClose( _glfw_window.get() );
   }

   void
   GLwindow::poolEvent()
   {
      glfwPollEvents();
   }

   void
   GLwindow::waitEvents()
   {
      glfwWaitEvents();
   }

   GLFWwindow*
   GLwindow::rawGLFWwindowPtr()
   {
      if ( !_glfw_window ) { utl::runtimeError( "glfw_window.get() is nullptr" ); }
      else { return _glfw_window.get(); }
      return nullptr;
   }

   const std::uint32_t&
   GLwindow::width()
   {
      return _width;
   }

   const std::uint32_t&
   GLwindow::height()
   {
      return _height;
   }

   const std::string&
   GLwindow::title()
   {
      return _title;
   }

   const bool
   GLwindow::framebufferResized()
   {
      return _framebufferResized;
   }

   void
   GLwindow::framebufferUnResized()
   {
      _framebufferResized = false;
   }

   const glm::u32vec2
   GLwindow::getFramebufferSize()
   {
      int w {}, h {};
      glfwGetFramebufferSize( _glfw_window.get(), &w, &h );
      return glm::u32vec2( w, h );
   }

   GLwindow::GLwindow( const std::uint32_t& width, const std::uint32_t& height, const std::string& title )
     : _width { width }, _height { height }, _title { title }
   {}

   GLwindow::~GLwindow() {}

}    // namespace my_library::window