#pragma once
#include "vkObjectTypes.hpp"
#include <glm/vec2.hpp>
namespace my_library::window
{
   class GLwindow;
   class GLwindowForVk
   {
   public:
      void
      waitEvents();
      const std::uint32_t&
      width();
      const std::uint32_t&
      height();
      const std::string&
      title();
      const bool
      framebufferResized();
      void
      framebufferUnResized();
      const std::vector<const char*>
      getRequiredInstanceExtensions() const;
      const VkResult
      glfwCreateWindowSurface( const VkInstance& instance, VkSurfaceKHR* surface );
      const glm::u32vec2
      getFramebufferSize();
      explicit GLwindowForVk( const std::unique_ptr<GLwindow>& window );
      GLwindowForVk();
      ~GLwindowForVk();
   private:
      const std::unique_ptr<GLwindow>& _window;
   };
}    // namespace window
