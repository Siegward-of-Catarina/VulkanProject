#pragma once
#include "../api/vulkan.hpp"
#include <iostream>
#include <vulkan/vulkan.h>
namespace lib::renderer
{
   class vulkan : public api::renderer::vulkan
   {
   private:
      ~vulkan() = default;
      void create_instace();
      void check_vk_extension_supported( const char** extensions, const std::uint32_t count );

   private:
      VkInstance vk_instance;

   public:
      void init() override;
      void release() override;
      vulkan();
   };
}    // namespace lib::renderer
