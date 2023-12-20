#pragma once
#include "../api/vulkan.hpp"
#include <vulkan/vulkan.h>
#include <vector>

namespace lib::renderer
{
   class vulkan : public api::renderer::vulkan
   {
   private:
      ~vulkan() = default;
      void create_instace();
      void check_extension_support( const char** extensions, const unsigned  count );
      bool check_validationlayer_support( std::vector<const char*> validationlayers );
   private:
      VkInstance vk_instance;

   public:
      void init() override;
      void release() override;
      vulkan();
   };
}    // namespace lib::renderer
