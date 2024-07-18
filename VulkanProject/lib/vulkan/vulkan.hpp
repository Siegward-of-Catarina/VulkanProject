#pragma once

#include "common.hpp"

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
   }    // namespace vulkan
}    // namespace my_library
namespace my_library
{
   namespace vulkan
   {
      struct queue_family_indices
      {
         std::optional<uint32_t> graphics_family;    // optional : Ç±ÇÍÇ≈ílÇ™ë∂ç›Ç∑ÇÈÇ©Ç«Ç§Ç©ÇîªíËÇ≈Ç´ÇÈ
         bool
         isComplete();
      };

      class vulkan
      {
      private:
         void
         pick_up_physical_device();
         void
         create_logical_device();
         bool
         check_validationlayer_support();
         bool
         is_device_suitable( VkPhysicalDevice device );
         int
         rate_device_suitability( VkPhysicalDevice device );
         queue_family_indices
         find_queue_familie( VkPhysicalDevice device );

      public:
         void
         init( const GLFWwindow* window );

         vulkan();
         ~vulkan();

      private:
         VkSurfaceKHR     vk_surface;
         VkPhysicalDevice vk_physical_device;
         VkDevice         vk_device;
         VkQueue          vk_graphics_queue;

         vk::DispatchLoaderDynamic dld;

         std::unique_ptr<instance>     _instance;
         std::unique_ptr<vulkan_debug> _vulkan_debug;

      };
   }    // namespace vulkan

}    // namespace my_library
