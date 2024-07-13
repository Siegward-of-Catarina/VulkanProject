#pragma once
#include "../create_helper.hpp"

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
   };

}

