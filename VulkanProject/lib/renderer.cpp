#include "renderer.hpp"

#include <GLFW/glfw3.h>

#include "vulkan/vulkan.hpp"

namespace my_library
{
   void
   renderer::init( const GLFWwindow* window )
   {
      _vulkan->init( window );
   }

   renderer::renderer() : _vulkan { std::make_unique<vulkan::vulkan>() } {}

   renderer::~renderer() {}

}    // namespace my_library
