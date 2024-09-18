#include "surface.hpp"

#include "../../utilities.hpp"
#include "../glwindowForVk.hpp"

#include <GLFW/glfw3.h>
namespace my_library::vkm
{
   void
   my_library::vkm::Surface::create( const UniqueInstance& instance )
   {
      VkSurfaceKHR surface;

      if ( _window->glfwCreateWindowSurface( static_cast<VkInstance>( instance.get() ), &surface ) != VK_SUCCESS )
      {
         utl::runtimeError( "failed to create window surface!" );
      }
      _surface = vkm::UniqueSurface( surface, { instance.get() } );
      utl::log( "create window surface succeeded." );
   }
   Surface::Surface( const std::unique_ptr<my_library::window::GLwindowForVk>& window ) 
       : _window { window }, _surface{}, vkobj {_surface}
   {}
   Surface::~Surface() {}
}    // namespace my_library::vkm
