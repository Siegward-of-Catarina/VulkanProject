#pragma once
#include "create_helper.hpp"

#include <optional>
#include <vector>
#include <vulkan/vulkan.h>
namespace my_library::window
{
   class glwindow;
}
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
      private:    // static
         static VKAPI_ATTR VkBool32 VKAPI_CALL
         debag_callBack( VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
                         VkDebugUtilsMessageTypeFlagsEXT             message_type,
                         const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
                         void*                                       user_data_ptr );

      private:
         vulkan();
         ~vulkan() = default;
         void
         create_instace();
         void
         setup_debug_messenger();
         void
         pick_up_physical_device();
         void
         create_logical_device();
         std::vector<const char*>
         get_required_extensions( const bool enable_validationlayers );
         bool
         check_extension_support( const std::vector<const char*>& extensions );
         bool
         check_validationlayer_support( const std::vector<const char*>& validationlayers );
         bool
         is_device_suitable( VkPhysicalDevice device );
         int
         rate_device_suitability( VkPhysicalDevice device );
         queue_family_indices
         find_queue_familie( VkPhysicalDevice device );
         void
         populate_debug_messenger_create_info( VkDebugUtilsMessengerCreateInfoEXT& create_info );

      private:
         VkInstance               vk_instance;
         VkDebugUtilsMessengerEXT vk_debug_messenger;
         VkSurfaceKHR             vk_surface;
         VkPhysicalDevice         vk_physical_device;
         VkDevice                 vk_device;
         VkQueue                  vk_graphics_queue;

         CREATE_HELPER;
         using create_helper = Impl<vulkan>;

      public:
         static vulkan*
         create();
      public:
         void
         init( const my_library::window::glwindow& window );
         void
         release();
      };
   }    // namespace vulkan

}    // namespace my_library
