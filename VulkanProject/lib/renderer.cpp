#include "renderer.hpp"
#include "./vulkan/initializeInfo.hpp"

#include <GLFW/glfw3.h>

#include "vulkan/vulkan.hpp"

namespace my_library
{
   void
   Renderer::init( std::unique_ptr<my_library::window::GLwindow>& gl_window, const bool debug )
   {
      vkm::vulkan::CreateInfo vulkan_create_info { gl_window,"triangle" };
      _vulkan->init( vulkan_create_info, debug );
   }

   Renderer::Renderer() : _vulkan { std::make_unique<vkm::Vulkan>() } {}

   Renderer::~Renderer() {}

}    // namespace my_library
