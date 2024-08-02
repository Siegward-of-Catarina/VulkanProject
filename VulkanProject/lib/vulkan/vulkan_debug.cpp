#include "vulkan_debug.hpp"

#include "../../pch.hpp"
#include "utilities.hpp"

namespace
{
   VKAPI_ATTR VkBool32 VKAPI_CALL
   debag_callBack( VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
                   VkDebugUtilsMessageTypeFlagsEXT             message_type,
                   const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
                   void*                                       user_data_ptr )
   {
      std::cerr << "validation layer:" << callback_data_ptr->pMessage << std::endl;
      return VK_FALSE;
   }
}    // namespace
namespace my_library
{
   namespace vulkan
   {
      namespace debug_utl
      {
         const vk_dbg_utl_msgr_createinfo_ext
         messenger_create_info()
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
      }    // namespace debug
   }       // namespace vulkan
}    // namespace my_library
