#pragma once

#include "common.hpp"

#include <vector>

namespace my_library::vulkan
{

   class instance
   {
   public:
      void
      init( std::string                                 app_name,
            const std::vector<const char*>&             validationlayers,
            const vk::DebugUtilsMessengerCreateInfoEXT& d_info,
            vk::DispatchLoaderDynamic&                  dld );

      void
      init( std::string app_name, vk::DispatchLoaderDynamic& dld );

      
      unq_vk_instance&
      vk_instance();

      instance();
      ~instance();

   private:
      unq_vk_instance _vk_instance;
   };

}    // namespace my_library::vulkan
