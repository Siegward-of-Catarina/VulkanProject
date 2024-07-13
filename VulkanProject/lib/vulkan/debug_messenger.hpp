#pragma once
#include "../create_helper.hpp"

#include <vector>
#include <vulkan/vulkan.h>
namespace my_library::vulkan
{
   class debug_messenger
   {
   private:
      static VKAPI_ATTR VkBool32 VKAPI_CALL
      debag_callBack( VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
                      VkDebugUtilsMessageTypeFlagsEXT             message_type,
                      const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
                      void*                                       user_data_ptr );

   public:
      static debug_messenger*
      create();

   private:
      void
      populate_debug_messenger_create_info( VkDebugUtilsMessengerCreateInfoEXT& create_info );

      debug_messenger();
   public:
      void
      register_for_create_info( VkInstanceCreateInfo& create_info, const std::vector<const char*>& validationlayers );
      void
      setup_messenger( VkInstance& vk_instance );
      void
      release( VkInstance& vk_instance );
   private:
      
      VkDebugUtilsMessengerEXT vk_debug_messenger;

      CREATE_HELPER;
      using create_helper = Impl<debug_messenger>;
   };
}    // namespace my_library::vulkan
