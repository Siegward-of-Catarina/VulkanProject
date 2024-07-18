#pragma once

#include "common.hpp"

namespace my_library::vulkan
{
   class vulkan_debug
   {
   public:
      static VKAPI_ATTR VkBool32 VKAPI_CALL
      debag_callBack( VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
                      VkDebugUtilsMessageTypeFlagsEXT             message_type,
                      const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
                      void*                                       user_data_ptr );

   public:
      vk::DebugUtilsMessengerCreateInfoEXT
      messenger_create_info();

      void
      setup_messenger( UniqueInstance& instance, vk::DispatchLoaderDynamic& dld );

      vulkan_debug() {}
      ~vulkan_debug() {}

   private:
      UniqueDebugUtilsMessengerEXT vk_debug_messenger;
   };
}    // namespace my_library::vulkan
