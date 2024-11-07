#include "glwindowForVk.hpp"

#include "../glwindow.hpp"

#define GLFW_INCLUDE_VULKAN    // GLFW には独自の定義が含まれ、それとともに Vulkan ヘッダーが自動的にロード
#include <GLFW/glfw3.h>

namespace my_library::window
{
   void
   GLwindowForVk::waitEvents()
   {
      _window->waitEvents();
   }
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
   const bool
   GLwindowForVk::framebufferResized()
   {
      return _window->framebufferResized();
   }
   void
   GLwindowForVk::framebufferUnResized()
   {
      _window->framebufferUnResized();
   }
   const std::vector<const char*>
   GLwindowForVk::getRequiredInstanceExtensions() const
   {
      uint32_t     glfw_extension_count { 0 };
      const char** glfw_extensions;
      glfw_extensions = glfwGetRequiredInstanceExtensions( &glfw_extension_count );
      std::vector<const char*> extensions { glfw_extensions, glfw_extensions + glfw_extension_count };
#ifdef __APPLE__    // MacOsに対応するため入れる
      extensions.emplace_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );
#endif
#ifdef _DEBUG
      extensions.emplace_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
#endif
      return extensions;
   }
   const VkResult
   GLwindowForVk::glfwCreateWindowSurface( const VkInstance& instance, VkSurfaceKHR* surface )
   {
      return ::glfwCreateWindowSurface( instance, _window->rawGLFWwindowPtr(), nullptr, surface );
   }
   const glm::u32vec2
   GLwindowForVk::getFramebufferSize()
   {
      return _window->getFramebufferSize();
   }
   GLwindowForVk::GLwindowForVk( const std::unique_ptr<GLwindow>& window ) : _window { window } {}
   GLwindowForVk::GLwindowForVk() : _window {} {}
   GLwindowForVk::~GLwindowForVk() {}
}    // namespace my_library::window
