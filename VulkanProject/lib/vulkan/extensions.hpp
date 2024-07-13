#pragma once
#include "../create_helper.hpp"
#include<vector>
#include <vulkan/vulkan.h>

namespace my_library::vulkan
{
   static class extensions
   {
   public://static
      static void
      register_for_create_info( VkInstanceCreateInfo& create_info );

   private://method

      static bool
      check_extension_support( const std::vector<const char*>& extensions );
   };
}
