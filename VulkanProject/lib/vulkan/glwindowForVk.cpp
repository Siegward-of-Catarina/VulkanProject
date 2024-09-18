#include "../glwindow.hpp"
#include "glwindowForVk.hpp"

#define GLFW_INCLUDE_VULKAN    // GLFW には独自の定義が含まれ、それとともに Vulkan ヘッダーが自動的にロード
#include <GLFW/glfw3.h>

namespace my_library::window
{
   const std::uint32_t&
   GLwindowForVk::width()
   {
      return _window->width();
   }
   const std::uint32_t&
   GLwindowForVk::height()
   {
      return _window->height();
   }
   const std::string&
   GLwindowForVk::title()
   {
      return _window->title();
   }
   const std::vector<const char*>
   GLwindowForVk::getRequiredInstanceExtensions( const bool debug ) const
   {
      uint32_t     glfw_extension_count { 0 };
      const char** glfw_extensions;
      glfw_extensions = glfwGetRequiredInstanceExtensions( &glfw_extension_count );
      std::vector<const char*> extensions { glfw_extensions, glfw_extensions + glfw_extension_count };
#ifdef __APPLE__    // MacOsに対応するため入れる
      extensions.emplace_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );
#endif
      if ( debug ) extensions.emplace_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
      return extensions;
   }
   const VkResult
   GLwindowForVk::glfwCreateWindowSurface( const VkInstance& instance, VkSurfaceKHR* surface )
   {
      return ::glfwCreateWindowSurface( instance, _window->rawGLFWwindowPtr(), nullptr, surface );
   }
   GLwindowForVk::GLwindowForVk( const std::unique_ptr<GLwindow>& window ) : _window { window } {}
   GLwindowForVk::GLwindowForVk() : _window {}{}
   GLwindowForVk::~GLwindowForVk() {}
}    // namespace my_library::window
