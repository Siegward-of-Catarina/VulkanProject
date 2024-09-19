#pragma once

#include "vkObjectTypes.hpp"

#include <memory>
#include <optional>
#include <vector>
namespace my_library
{
   namespace window
   {
      class GLwindowForVk;
   }

   namespace vkm
   {
      namespace vulkan
      {
         struct CreateInfo;
      }

      class VkObjContainer;
      class Instance;
      class Surface;
      class Device;
      class Vulkan
      {
      public:
         void
         init( const vulkan::CreateInfo& create_info, const bool debug = false );
         Vulkan();
         ~Vulkan();

      private:
         std::unique_ptr<Instance> _instance;
         std::unique_ptr<Surface>  _surface;
         std::unique_ptr<Device>  _device;
      };
   }    // namespace vkm

}    // namespace my_library
