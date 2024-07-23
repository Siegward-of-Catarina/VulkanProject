#pragma once

#include "vk_object_types.hpp"

#include <vector>

namespace my_library::vulkan
{

   class instance
   {
   public:
      void
      init( std::string                                   app_name,
            const std::vector<const char*>&               validationlayers,
            const vk_debugutils_messenger_createinfo_ext& d_info,
            vk_dispatchloader_dynamic&                    dld );

      void
      init( std::string app_name, vk_dispatchloader_dynamic& dld );

      unq_vk_instance&
      vk_obj();

      instance();
      ~instance();

   private:
      unq_vk_instance _vk_instance;
   };

}    // namespace my_library::vulkan
