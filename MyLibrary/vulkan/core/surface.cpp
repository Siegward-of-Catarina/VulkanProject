#include "surface.hpp"

#include "../../pch.hpp"
#include "../../utilities.hpp"
#include "../glwindowForVk.hpp"

#include <GLFW/glfw3.h>
namespace my_library::vkm
{
   void
   my_library::vkm::Surface::create( const instance::UniqueHandle&instance )
   {
      VkSurfaceKHR surface {};

      if ( window->glfwCreateWindowSurface( static_cast<VkInstance>( instance.get() ), &surface ) != VK_SUCCESS )
      {
         utl::runtimeError( "failed to create window surface!" );
      }
      _surface = vkm::surface::UniqueHandle( surface, { instance.get() } );
      utl::log( "create window surface succeeded." );
   }
   Surface::Surface( const std::unique_ptr<my_library::window::GLwindowForVk>& _window )
     : window { _window }, _surface {}, vk_surface { _surface }
   {}
   Surface::~Surface() {}
}    // namespace my_library::vkm
