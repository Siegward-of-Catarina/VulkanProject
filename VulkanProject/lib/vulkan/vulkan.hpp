#pragma once

#include "vk_object_types.hpp"

#include <memory>
#include <optional>
#include <vector>
struct GLFWwindow;
namespace my_library
{
   namespace vulkan
   {
      class instance;
      class vulkan_debug;
      class device;
   }    // namespace vulkan
}    // namespace my_library
namespace my_library
{
   namespace vulkan
   {
      class vulkan
      {
      private:
         bool
         check_validationlayer_support();

      public:
         void
         init( const GLFWwindow* window );

         vulkan();
         ~vulkan();

      private:
         vk_dispatchloader_dynamic dld;
         vk_queue                      _graphics_queue;
         std::unique_ptr<instance>     _instance;
         std::unique_ptr<vulkan_debug> _vulkan_debug;
         std::unique_ptr<device> _device;

      };
   }    // namespace vulkan

}    // namespace my_library
