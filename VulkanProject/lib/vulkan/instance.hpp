#pragma once

#include "vk_object_types.hpp"

#include <vector>

namespace my_library::vulkan
{

   class instance
   {
   public:
      void
      init( const std::string                                   app_name,
            const vk_dbg_utl_msgr_createinfo_ext& d_info,
            vk_dispatchloader_dynamic&                    dld );

      void
      init( const std::string app_name, vk_dispatchloader_dynamic& dld );

      const unq_vk_instance&
      vk_obj();

      instance();
      ~instance();

   private:
      unq_vk_instance _vk_instance;
   };

}    // namespace my_library::vulkan
