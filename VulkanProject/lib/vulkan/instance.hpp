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

      
      UniqueInstance&
      unq_vk_instance();

      instance();
      ~instance();

   private:
      UniqueInstance vk_instance;
   };

}    // namespace my_library::vulkan
