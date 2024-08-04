#pragma once
#include "vkObjectTypes.hpp"
namespace my_library
{
   namespace window
   {
      class GLwindow;
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
            std::unique_ptr<my_library::window::GLwindow>& window;
            std::string      app_name;
         };
      }    // namespace vulkan
      namespace instance
      {
         struct CreateInfo
         {
            std::string                      app_name;
            debug_utl::MessengerCreateInfoEXT debug_info;
            std::vector<const char*>         extensions;
            std::vector<const char*>         validation_layers;
            DispatchLoaderDynamic            dld;
         };
      }    // namespace instance
   }       // namespace vkm
}    // namespace my_library