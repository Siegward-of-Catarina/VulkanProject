#include "renderer.hpp"

#include "./vulkan/glwindowForVk.hpp"
#include "./vulkan/initializeInfo.hpp"
#include "vulkan/vulkan.hpp"

#include <GLFW/glfw3.h>
#include <iostream>
namespace
{
   namespace renderer = my_library::renderer;
   namespace vkm      = my_library::vkm;

   vkm::DebugUtilsMesseagSeverityFlagsEXT
   convertVulkanSeverityFlags( const my_library::enumExtensions::BitFlag<renderer::DebugSeverityFlagBits> flags )
   {
      vkm::DebugUtilsMesseagSeverityFlagsEXT convert_flags;
      if ( flags.validFlag( renderer::DebugSeverityFlagBits::Error ) )
      {
         convert_flags = vkm::DebugUtilsMessageSeverityFlagBitsEXT::eError;
      }
      if ( flags.validFlag( renderer::DebugSeverityFlagBits::Warning ) )
      {
         convert_flags |= vkm::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;
      }
      if ( flags.validFlag( renderer::DebugSeverityFlagBits::Verbose ) )
      {
         convert_flags |= vkm::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose;
      }

      return convert_flags;
   }
   vkm::DebugUtilsMessageTypeFlagsEXT
   convertVulkanMessageTypeFlags( const my_library::enumExtensions::BitFlag<renderer::DebugMessageTypeFlagBits> flags )
   {
      vkm::DebugUtilsMessageTypeFlagsEXT convert_flags;

      if ( flags.validFlag( renderer::DebugMessageTypeFlagBits::General ) )
      {
         convert_flags = vkm::DebugUtilsMessageTypeFlagBitsEXT::eGeneral;
      }
      if ( flags.validFlag( renderer::DebugMessageTypeFlagBits::Performance ) )
      {
         convert_flags |= vkm::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
      }
      if ( flags.validFlag( renderer::DebugMessageTypeFlagBits::Validation ) )
      {
         convert_flags |= vkm::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
      }

      return convert_flags;
   }
}    // namespace
namespace my_library
{
   void
   Renderer::init( const renderer::CreateInfo& create_info )
   {
      vkm::vulkan::CreateInfo vulkan_create_info {
         .window { std::make_unique<window::GLwindowForVk>( create_info.gl_window ) },
         .app_name { create_info.app_name },
         .debug_severity_flags { convertVulkanSeverityFlags( create_info.debug_severity_flags ) },
         .debug_msg_type_flags { convertVulkanMessageTypeFlags( create_info.debug_msg_type_flags ) }
      };
      _vulkan->init( vulkan_create_info, create_info.enable_debug );
   }

   Renderer::Renderer() : _vulkan { std::make_unique<vkm::Vulkan>() } {}

   Renderer::~Renderer() {}

}    // namespace my_library
