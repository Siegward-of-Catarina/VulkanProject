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
                      const VkDebugUtilsMessengerCallbackDataEXT* pCallback_data,
                      void*                                       pUser_data );
   private:
      ~vulkan() = default;
      void
      create_instace();

      VkInstanceCreateInfo
      get_vkinstance_create_info( const VkApplicationInfo*        app_info,
                                  const std::vector<const char*>& extensions,
                                  const std::vector<const char*>& validationlayers );
      std::vector<const char*>
      get_required_extensions( const bool enable_validationlayers );
      bool
      check_extension_support( const std::vector<const char*>& extensions );
      bool
      check_validationlayer_support( const std::vector<const char*>& validationlayers );

   private:
      VkInstance vk_instance;

   public:
      void
      init() override;
      void
      release() override;
      vulkan();
   };
}    // namespace lib::renderer
