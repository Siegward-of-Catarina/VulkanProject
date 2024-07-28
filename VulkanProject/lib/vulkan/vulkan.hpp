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
      class surface;
      class physicaldevice;
      class logicaldevice;
   }    // namespace vulkan
}    // namespace my_library
namespace my_library
{
   namespace vulkan
   {
      class vulkan
      {
      public:
         void
         init( GLFWwindow* window, const bool debug = false );

         vulkan();
         ~vulkan();

      private:
         vk_dispatchloader_dynamic             dld;
         vk_queue                              _graphics_queue;
         vk_queue                              _present_queue;
         const std::unique_ptr<instance>       _instance;
         const std::unique_ptr<vulkan_debug>   _vulkan_debug;
         const std::unique_ptr<surface>        _surface;
         const std::unique_ptr<physicaldevice> _physicaldevice;
         const std::unique_ptr<logicaldevice>  _logicaldevice;
      };
   }    // namespace vulkan

}    // namespace my_library
