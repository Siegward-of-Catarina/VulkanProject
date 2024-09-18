#pragma once
#include "vkObjectTypes.hpp"
#include "debugUtils.hpp"

#include <optional>
namespace my_library
{
   namespace window
   {
      class GLwindowForVk;
   }
}    // namespace my_library

namespace my_library
{
   namespace vkm
   {
      namespace vulkan
      {
         struct CreateInfo
         {
            std::unique_ptr<my_library::window::GLwindowForVk> window;
            std::string                                        app_name { "my_app" };
            DebugUtilsMesseagSeverityFlagsEXT                  debug_severity_flags {};
            DebugUtilsMessageTypeFlagsEXT                      debug_msg_type_flags {};
         };
      }    // namespace vulkan
      
   }       // namespace vkm
}    // namespace my_library