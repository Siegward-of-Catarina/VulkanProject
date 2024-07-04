#pragma once
#include "create_helper.hpp"

#include<vector>
#include <vulkan/vulkan.h>
namespace my_library::vulkan
{
   class instance
   {
   private:
      VkInstance vk_instance;

      CREATE_HELPER;
      using create_helper = Impl<instance>;

   public:
      static instance*
      create();
   private:
      instance();

      std::vector<const char*>
      get_required_extensions( const bool enable_validationlayers );
      bool
      check_extension_support( const std::vector<const char*>& extensions );
      bool
      check_validationlayer_support( const std::vector<const char*>& validationlayers );
   };

}

