#pragma once
#include "vkObjectTypes.hpp"
namespace my_library::window
{
   class GLwindow;
   class GLwindowForVk
   {
   public:
      const std::uint32_t&
      width();
      const std::uint32_t&
      height();
      const std::string&
      title();
      const std::vector<const char*>
      getRequiredInstanceExtensions( const bool debug ) const;
      const VkResult
      glfwCreateWindowSurface( const VkInstance& instance, VkSurfaceKHR* surface );
      explicit GLwindowForVk( const std::unique_ptr<GLwindow>& window );
      GLwindowForVk();
      ~GLwindowForVk();
   private:
      const std::unique_ptr<GLwindow>& _window;
   };
}    // namespace window
