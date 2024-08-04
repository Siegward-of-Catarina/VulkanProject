#include "debugUtils.hpp"

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
   namespace vkm
   {
      namespace debug_utl
      {
         const PFN_vkDebugUtilsMessengerCallbackEXT
         callBack()
         {
            return &debag_callBack;
         }

         const MessengerCreateInfoEXT
         populateCreateInfo()
         {
            // in debug mode, use the debugUtilsMessengerCallback
            const MesseagSeverityFlagsEXT severityFlags { MessageSeverityFlagBitsEXT::eWarning
                                                                    | MessageSeverityFlagBitsEXT::eError
                                                                    | MessageSeverityFlagBitsEXT::eVerbose };

            const MessageTypeFlagsEXT messageTypeFlags { MessageTypeFlagBitsEXT::eGeneral
                                                                   | MessageTypeFlagBitsEXT::ePerformance
                                                                   | MessageTypeFlagBitsEXT::eValidation };

            return MessengerCreateInfoEXT { {}, severityFlags, messageTypeFlags, &debag_callBack };
         }
      }    // namespace debug_utl
   }       // namespace vkm
}    // namespace my_library
