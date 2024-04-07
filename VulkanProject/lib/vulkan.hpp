#pragma once
#include "../api/vulkan.hpp"

#include <vector>   
#include <vulkan/vulkan.h>

namespace lib::renderer
{
   class vulkan : public api::renderer::vulkan
   {
   private://static
      static VKAPI_ATTR VkBool32 VKAPI_CALL
      debag_callBack( VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
                      VkDebugUtilsMessageTypeFlagsEXT             message_type,
                      const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
                      void*                                       user_data_ptr );
   private:
      ~vulkan() = default;
      void
      create_instace();
      void
      setup_debug_messenger();

      std::vector<const char*>
      get_required_extensions( const bool enable_validationlayers );
      bool
      check_extension_support( const std::vector<const char*>& extensions );
      bool
      check_validationlayer_support( const std::vector<const char*>& validationlayers );
      void
      populate_debug_messenger_create_info( VkDebugUtilsMessengerCreateInfoEXT& create_info );

   private:
      VkInstance vk_instance;
      VkDebugUtilsMessengerEXT vk_debug_messenger;
   public:
      void
      init() override;
      void
      release() override;
      vulkan();
   };
}    // namespace lib::renderer
