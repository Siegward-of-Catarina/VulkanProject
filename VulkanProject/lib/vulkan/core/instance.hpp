#pragma once
#include "core_commonheaders.hpp"

namespace my_library::vulkan
{

   class instance : public vkobject
   {
   public:
      void
      init( const std::string app_name, const bool debug );

      instance( const shared_container& c );
      ~instance();

   private:
      unq_vk_dbg_utl_msgr_ext _vk_dbg_magr;
   };

}    // namespace my_library::vulkan
