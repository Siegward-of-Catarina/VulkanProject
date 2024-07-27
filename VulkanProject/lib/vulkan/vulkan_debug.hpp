#pragma once

#include "vk_object_types.hpp"

namespace my_library::vulkan
{
   class vulkan_debug
   {
   public:
       //static ‚È‚Ì‚Å using ‚³‚ê‚½Œ^–¼‚ªg‚¦‚È‚¢.‚æ‚Á‚Ä¶‚ÌŒ^–¼‚ğg—p
      static VKAPI_ATTR VkBool32 VKAPI_CALL
      debag_callBack( VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
                      VkDebugUtilsMessageTypeFlagsEXT             message_type,
                      const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
                      void*                                       user_data_ptr );

   public:
      const vk_dbg_utl_msgr_createinfo_ext
      messenger_create_info();

      void
      setup_messenger( const unq_vk_instance& instance, const vk_dispatchloader_dynamic& dld );

      vulkan_debug() {}
      ~vulkan_debug() {}

   private:
      unq_vk_dbg_utl_msgr_ext _vk_debug_messenger;
   };
}    // namespace my_library::vulkan
