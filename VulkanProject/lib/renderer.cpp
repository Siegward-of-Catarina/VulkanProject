#include "renderer.hpp"
#include "glwindow.hpp"
#include "vulkan.hpp"
namespace my_library
{
   renderer::renderer( const std::uint32_t width, const std::uint32_t height, const char* title ) 
       : window { window::glwindow::create( width, height, title ) }, vulkan { vulkan::vulkan::create() }
   {}

   renderer::~renderer() {}

   renderer*
   renderer::create( const std::uint32_t width, const std::uint32_t height, const char* title )
   {
      return new create_helper(width, height, title);
   }

   void
   renderer::init()
   {
      window->init();
      vulkan->init(*window);
   }

   void
   renderer::release()
   {
      vulkan->release();
      window->release();
   }
}

