#pragma once
#include "../vkObjectTypes.hpp"
namespace my_library::window
{
   class GLwindowForVk;
}
namespace my_library::vkm
{
   class Surface
   {
   public:
      void
      create( const instance::UniqueHandle& instance );

      Surface( const std::unique_ptr<my_library::window::GLwindowForVk>& _window );
      ~Surface();

   public:
      const surface::UniqueHandle& vk_surface;
      const std::unique_ptr<my_library::window::GLwindowForVk>& window;
   private:
      surface::UniqueHandle                                             _surface;
   };
}    // namespace my_library::vulkan
