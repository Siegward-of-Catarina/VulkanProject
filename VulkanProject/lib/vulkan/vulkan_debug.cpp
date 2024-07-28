#include "vulkan_debug.hpp"

#include "utilities.hpp"

#include <iostream>

namespace my_library::vulkan
{
   VKAPI_ATTR VkBool32 VKAPI_CALL
   vulkan_debug::debag_callBack( VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
                                 VkDebugUtilsMessageTypeFlagsEXT             message_type,
                                 const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
                                 void*                                       user_data_ptr )
   {
      std::cerr << "validation layer:" << callback_data_ptr->pMessage << std::endl;
      return VK_FALSE;
   }

   const vk_dbg_utl_msgr_createinfo_ext
   vulkan_debug::messenger_create_info()
   {
      // in debug mode, use the debugUtilsMessengerCallback
      const vk_dbg_utl_msg_severity_flags_ext severityFlags { vk_dbg_utl_msg_severity_flag_bits_ext::eWarning
                                                              | vk_dbg_utl_msg_severity_flag_bits_ext::eError
                                                              | vk_dbg_utl_msg_severity_flag_bits_ext::eVerbose };

      const vk_dbg_utl_msg_type_flags_ext messageTypeFlags { vk_dbg_utl_msg_type_flag_bits_ext::eGeneral
                                                             | vk_dbg_utl_msg_type_flag_bits_ext::ePerformance
                                                             | vk_dbg_utl_msg_type_flag_bits_ext::eValidation };

      return vk_dbg_utl_msgr_createinfo_ext { {}, severityFlags, messageTypeFlags, &debag_callBack };
   }

   void
   vulkan_debug::setup_messenger( const unq_vk_instance& instance, const vk_dispatchloader_dynamic& dld )
   {
      _vk_debug_messenger = instance->createDebugUtilsMessengerEXTUnique( messenger_create_info(), nullptr, dld );
      utl::log( "DebugUtils Messenger setup completed." );
   }

}    // namespace my_library::vulkan