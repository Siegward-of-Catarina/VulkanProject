#include "surface.hpp"

#include "utilities.hpp"

#include <GLFW/glfw3.h>
namespace my_library::vulkan
{
   void
   surface::init( const unq_vk_instance& instance, GLFWwindow* window )
   {
      VkSurfaceKHR surface;
      if ( glfwCreateWindowSurface( static_cast<VkInstance>( instance.get() ), window, nullptr, &surface )
           != VK_SUCCESS )
      {
         throw std::runtime_error( "failed to create window surface!" );
      }
      _surface = unq_vk_surface( surface, { instance.get() } );

      utl::log( "create window surface succeeded." );
   }

   const unq_vk_surface&
   surface::vk_obj()
   {
      return _surface;
   }

   surface::surface() {}
   surface::~surface() {}
}    // namespace my_library::vulkan
