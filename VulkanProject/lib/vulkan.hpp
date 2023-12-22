#pragma once
#include "../api/vulkan.hpp"

#include <vector>
#include <vulkan/vulkan.h>

namespace lib::renderer
{
   class vulkan : public api::renderer::vulkan
   {
   private:
      ~vulkan() = default;
      void                     create_instace();
      void                     get_vkinstance_create_info( VkApplicationInfo* app_info, VkInstanceCreateInfo* create_info );
      std::vector<const char*> get_required_extensions( const bool enable_validationlayers );
      bool                     check_extension_support( std::vector<const char*> extensions );
      bool                     check_validationlayer_support( std::vector<const char*> validationlayers );

   private:
      VkInstance vk_instance;

   public:
      void init() override;
      void release() override;
      vulkan();
   };
}    // namespace lib::renderer
