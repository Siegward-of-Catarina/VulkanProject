#include "glwindow.hpp"

#define GLFW_INCLUDE_VULKAN    // GLFW には独自の定義が含まれ、それとともに Vulkan ヘッダーが自動的にロード
#include <GLFW/glfw3.h>

namespace my_library::window
{
   glwindow*
   glwindow::create( const std::uint32_t width, const std::uint32_t height, const char* title )
   {
      return new create_helper(width,height,title);
   }

   glwindow::glwindow( const std::uint32_t width, const std::uint32_t height, const char* title )
     : glfw_window { nullptr }, width { width }, height { height }, title { title }
   {}

   glwindow::~glwindow() {}

   int
   glwindow::close()
   {
      return glfwWindowShouldClose( glfw_window );
   }

   void
   glwindow::pool_event()
   {
      glfwPollEvents();
   }

   void
   glwindow::release()
   {
      glfwDestroyWindow( glfw_window );
      glfwTerminate();
      delete this;
   }

   const GLFWwindow*
   glwindow::get_window()
   {
      if ( glfw_window == nullptr ) { throw std::runtime_error( "glfw_window is nullptr" ); }
      else { return glfw_window; }
      return nullptr;
   }

   const std::uint32_t
   glwindow::get_width()
   {
      return width;
   }

   const std::uint32_t
   glwindow::get_height()
   {
      return height;
   }

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
      glfw_window = glfwCreateWindow( width, height, title, nullptr, nullptr );
   }
}    // namespace lib::window