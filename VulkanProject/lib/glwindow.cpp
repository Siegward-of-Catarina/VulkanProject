#include "glwindow.hpp"

#define GLFW_INCLUDE_VULKAN    // GLFW には独自の定義が含まれ、それとともに Vulkan ヘッダーが自動的にロード
#include <GLFW/glfw3.h>

namespace my_library::window    // deleter
{
   void
   gl_deleter::operator()( GLFWwindow* ptr )
   {
      glfwDestroyWindow( ptr );
      glfwTerminate();
   }
}    // namespace my_library::window

namespace my_library::window    // glwindow
{
   void
   glwindow::init()
   {
      // glfw 初期化
      glfwInit();

      // OpenGLコンテキストを使用しないように指定
      glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
      // ウィンドウサイズ変更は、ややこしいから今は、無効
      glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

      // ウィンドウ初期化
      //_glfw_window.reset( glfwCreateWindow( _width, _height, _title, nullptr, nullptr ) );
      std::unique_ptr<GLFWwindow, gl_deleter> w { glfwCreateWindow( _width, _height, _title, nullptr, nullptr ) };
      _glfw_window = std::move( w );
   }

   int
   glwindow::close()
   {
      return glfwWindowShouldClose( _glfw_window.get() );
   }

   void
   glwindow::pool_event()
   {
      glfwPollEvents();
   }

   const GLFWwindow*
   glwindow::raw_glfwW_ptr()
   {
      if ( !_glfw_window ) { throw std::runtime_error( "glfw_window.get() is nullptr" ); }
      else { return _glfw_window.get(); }
      return nullptr;
   }

   const std::uint32_t
   glwindow::width()
   {
      return _width;
   }

   const std::uint32_t
   glwindow::height()
   {
      return _height;
   }

   glwindow::glwindow( const std::uint32_t width, const std::uint32_t height, const char* title )
     : _width { width }, _height { height }, _title { title }
   {}

   glwindow::~glwindow() {}

}    // namespace my_library::window