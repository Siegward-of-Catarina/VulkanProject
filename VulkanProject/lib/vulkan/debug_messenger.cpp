#include "debug_messenger.hpp"
namespace
{
   VkResult    // VkDebugUtilsMessengerEXTを作成する関数を探し出す
   create_debug_utils_messenger_ext( VkInstance                                instance,
                                     const VkDebugUtilsMessengerCreateInfoEXT* create_info_ptr,
                                     const VkAllocationCallbacks*              allocator_ptr,
                                     VkDebugUtilsMessengerEXT*                 debug_messenger_ptr )
   {
      auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT" ) );
      if ( func != nullptr ) { return func( instance, create_info_ptr, allocator_ptr, debug_messenger_ptr ); }
      else { return VK_ERROR_EXTENSION_NOT_PRESENT; }
   }

   void    // VkDebugUtilsMessengerEXTを解放する関数を探し出す
   destroy_debug_utils_messenger_ext( VkInstance                   instance,
                                      VkDebugUtilsMessengerEXT     debug_messenger,
                                      const VkAllocationCallbacks* allocator_ptr )
   {
      auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr( instance, "vkDestroyDebugUtilsMessengerEXT" ) );
      if ( func != nullptr ) { func( instance, debug_messenger, allocator_ptr ); }
   }
}    // namespace
namespace my_library
{
   /*
     VkDebugUtilsMessageSeverityFlagBitsEXT :　メッセージの重要度
     VkDebugUtilsMessageTypeFlagsEXT : メッセージの種類
     const VkDebugUtilsMessageCallbackDataEXT* : メッセージの詳細を含む構造体、pCallbackDataを参照。
    */
   VKAPI_ATTR VkBool32 VKAPI_CALL
   vulkan::debug_messenger::debag_callBack( VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
                                            VkDebugUtilsMessageTypeFlagsEXT             message_type,
                                            const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
                                            void*                                       user_data_ptr )
   {
      std::cerr << "validation layer:" << callback_data_ptr->pMessage << std::endl;
      return VK_FALSE;
   }

   vulkan::debug_messenger*
   vulkan::debug_messenger::create()
   {
      return new create_helper();
   }

   void
   vulkan::debug_messenger::populate_debug_messenger_create_info( VkDebugUtilsMessengerCreateInfoEXT& create_info )
   {
      create_info                 = {};
      create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT    // より詳細な情報
                                  | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT    // 警告
                                  | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;     // エラー
      create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    // いったんすべての種類を表示
                              | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                              | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      create_info.pfnUserCallback = debag_callBack;
      create_info.pUserData       = nullptr;
   }

   vulkan::debug_messenger::debug_messenger() : vk_debug_messenger { VK_NULL_HANDLE } {}

   void
   vulkan::debug_messenger::register_for_create_info( VkInstanceCreateInfo&           create_info,
                                                      const std::vector<const char*>& validationlayers )
   {
      // vkCreateInctance, vkDestroyInstanceの内部イベントに対応するためもう一つのdebugMessengerを用意する。
      VkDebugUtilsMessengerCreateInfoEXT debug_create_info {};
      create_info.enabledLayerCount   = static_cast<uint32_t>( validationlayers.size() );
      create_info.ppEnabledLayerNames = validationlayers.data();
      populate_debug_messenger_create_info( debug_create_info );
      // ここでpNextにdebug_create_infoを指定することでcreate_infoが拡張されデバッグできるようになる。
      create_info.pNext = reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT*>( &debug_create_info );
   }

   void
   vulkan::debug_messenger::setup_messenger( VkInstance& vk_instance )
   {

      VkDebugUtilsMessengerCreateInfoEXT create_info {};
      populate_debug_messenger_create_info( create_info );

      if ( create_debug_utils_messenger_ext( vk_instance, &create_info, nullptr, &vk_debug_messenger ) != VK_SUCCESS )
      {
         throw std::runtime_error( "failed to set up debug messenger!" );
      }
   }

   void
   vulkan::debug_messenger::release( VkInstance& vk_instance )
   {
      destroy_debug_utils_messenger_ext( vk_instance, vk_debug_messenger, nullptr );
      delete this;
   }

}    // namespace my_library
