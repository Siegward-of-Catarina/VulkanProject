#include "surface.hpp"

#include "../container.hpp"
#include "../utilities.hpp"

#include <GLFW/glfw3.h>
namespace my_library::vulkan
{
   void
   surface::init( GLFWwindow* window )
   {
      VkSurfaceKHR surface;
      if ( const shared_container c = ctr_lock() )
      {
         if ( glfwCreateWindowSurface( static_cast<VkInstance>( c->instance.get() ), window, nullptr, &surface )
              != VK_SUCCESS )
         {
            throw std::runtime_error( "failed to create window surface!" );
         }
         _surface = unq_vk_surface( surface, { c->instance.get() } );

         utl::log( "create window surface succeeded." );
      }
   }

   const unq_vk_surface&
   surface::vk_obj()
   {
      return _surface;
   }

   surface::surface( const shared_container& c ) : vkobject { c } {}
   surface::~surface() {}
}    // namespace my_library::vulkan
