#include "renderer.hpp"

#include "vulkan.hpp"
namespace my_library
{
   renderer::renderer( )
     : vulkan { vulkan::vulkan::create() }
   {}

   renderer::~renderer() {}

   renderer*
   renderer::create( )
   {
      return new create_helper( );
   }

   void
   renderer::init()
   {
      // vulkan->init(*window);
   }

   void
   renderer::release()
   {
      vulkan->release();
   }
}    // namespace my_library
