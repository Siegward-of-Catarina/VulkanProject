#pragma once

#include "vkObjectTypes.hpp"

#include <memory>
#include <optional>
#include <vector>
namespace my_library
{
   namespace window
   {
      class GLwindow;
   }
   namespace vkm
   {
      namespace vulkan
      {
         struct CreateInfo;
      }
      namespace instance
      {
         struct CreateInfo;
      }
      class Container;
      class Instance;

   }
}    // namespace my_library
namespace my_library
{
   namespace vkm
   {

      class Vulkan
      {
      public:
         void
         init( const vulkan::CreateInfo& create_info, const bool debug = false );

         Vulkan();
         ~Vulkan();

      private:

      private:
         std::shared_ptr<Container> _container;
         std::unique_ptr<Instance> _instance;
         Queue _graphics_queue;
         Queue _present_queue;
      };
   }    // namespace vkm

}    // namespace my_library
