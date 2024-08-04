#include "glwindow.hpp"

#define GLFW_INCLUDE_VULKAN    // GLFW �ɂ͓Ǝ��̒�`���܂܂�A����ƂƂ��� Vulkan �w�b�_�[�������I�Ƀ��[�h
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
   GLwindow::init()
   {
      // glfw ������
      glfwInit();

      // OpenGL�R���e�L�X�g���g�p���Ȃ��悤�Ɏw��
      glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
      // �E�B���h�E�T�C�Y�ύX�́A��₱�������獡�́A����
      glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

      // �E�B���h�E������
      //_glfw_window.reset( glfwCreateWindow( _width, _height, _title, nullptr, nullptr ) );
      std::unique_ptr<GLFWwindow, GLdeleter> w { glfwCreateWindow(
        _width, _height, _title.c_str(), nullptr, nullptr ) };
      _glfw_window = std::move( w );
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

   GLFWwindow*
   GLwindow::rawGLFWwindowPtr()
   {
      if ( !_glfw_window ) { throw std::runtime_error( "glfw_window.get() is nullptr" ); }
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

   const std::vector<const char*>
   GLwindow::getRequiredInstanceExtensions( const bool debug ) const
   {
      uint32_t     glfw_extension_count { 0 };
      const char** glfw_extensions;
      glfw_extensions = glfwGetRequiredInstanceExtensions( &glfw_extension_count );
      std::vector<const char*> extensions { glfw_extensions, glfw_extensions + glfw_extension_count };
#ifdef __APPLE__    // MacOs�ɑΉ����邽�ߓ����
      extensions.emplace_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );
#endif
      if( debug ) extensions.emplace_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
      return extensions;
   }

   GLwindow::GLwindow( const std::uint32_t& width, const std::uint32_t& height, const std::string& title )
     : _width { width }, _height { height }, _title { title }
   {}

   GLwindow::~GLwindow() {}

}    // namespace my_library::window