#pragma once
#include "../api/vulkan.hpp"
#include <iostream>
#include <vulkan/vulkan.h>
namespace std
{
   template<class T>
   class allocator;
   template<class T, class Allocator>
   class vector;
}

namespace lib::renderer
{
   class vulkan : public api::renderer::vulkan
   {
   private:
      ~vulkan() = default;
      void create_instace();
      void check_extension_support( const char** extensions, const std::uint32_t count );
      bool check_validationlayer_support( std::vector<const char*, std::allocator<const char*> > validationlayers );

   private:
      VkInstance vk_instance;

   public:
      void init() override;
      void release() override;
      vulkan();
   };
}    // namespace lib::renderer
