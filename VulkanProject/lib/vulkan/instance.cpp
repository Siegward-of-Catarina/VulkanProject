#include "instance.hpp"

#include "utilities.hpp"
#include "validationlayer.hpp"
#include "extension.hpp"

#include <GLFW/glfw3.h>    //拡張機能を取得するために必要
#include <iostream>
#include <string>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace my_library
{
   void
   vulkan::instance::init( const std::string                     app_name,
                           const vk_dbg_utl_msgr_createinfo_ext& d_info,
                           vk_dispatchloader_dynamic&            dld )
   {
      const vk_applicationinfo appInfo( app_name.c_str(),
                                        VK_MAKE_VERSION( 1, 0, 0 ),
                                        app_name.c_str(),
                                        VK_MAKE_VERSION( 1, 0, 0 ),
                                        VK_API_VERSION_1_2 );

      const auto extensions { ext::get_required_extensions( dld ) };


      const auto validationlayers { layer::get_validationlayers() };
      if ( !validationlayers.empty() )
      {
         const vk_structure_chain<vk_instance_createinfo, vk_dbg_utl_msgr_createinfo_ext> createInfo {
            {{}, &appInfo, validationlayers, extensions},
            d_info
         };

         _vk_instance = vk::createInstanceUnique( createInfo.get<vk_instance_createinfo>(), nullptr, dld );
      }
      else
      {
         const vk_instance_createinfo createInfo { {}, &appInfo, {}, extensions };
         _vk_instance = vk::createInstanceUnique( createInfo, nullptr, dld );
      }
      // 全ての関数ポインタを取得する
      dld.init( *_vk_instance );

      utl::log( "create instance succeeded." );
   }

   const vulkan::unq_vk_instance&
   vulkan::instance::vk_obj()
   {
      return _vk_instance;
   }

   vulkan::instance::instance() : _vk_instance {} {}

   vulkan::instance::~instance() {}
}    // namespace my_library
