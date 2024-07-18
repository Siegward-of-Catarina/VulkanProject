#include "vulkan_debug.hpp"

#include<iostream>

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

   vk::DebugUtilsMessengerCreateInfoEXT
   vulkan_debug::messenger_create_info()
   {
      // in debug mode, use the debugUtilsMessengerCallback
      vk::DebugUtilsMessageSeverityFlagsEXT severityFlags( vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
                                                           | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
                                                           | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose );

      vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags( vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
                                                          | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
                                                          | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation );

      return vk::DebugUtilsMessengerCreateInfoEXT( {}, severityFlags, messageTypeFlags, &debag_callBack );
   }

   void
   vulkan_debug::setup_messenger( UniqueInstance& instance, vk::DispatchLoaderDynamic& dld )
   {
      vk_debug_messenger = instance->createDebugUtilsMessengerEXTUnique( messenger_create_info(), nullptr, dld );
   }


}    // namespace my_library::vulkan