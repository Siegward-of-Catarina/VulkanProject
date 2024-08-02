#include "instance.hpp"

#include "../container.hpp"
#include "../extension.hpp"
#include "../utilities.hpp"
#include "../validationlayer.hpp"
#include "../vulkan_debug.hpp"

#include "../../../pch.hpp"

#include <GLFW/glfw3.h>    //拡張機能を取得するために必要

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
namespace
{
   namespace my = my_library::vulkan;

   my::unq_vk_instance
   create_dbg( const my::vk_applicationinfo& info, const my::vk_dispatchloader_dynamic& dld )
   {
      const auto extensions { my::ext::get_required_extensions( dld ) };

      const auto validationlayers { my::layer::get_validationlayers() };
      assert( !validationlayers.empty() );

      const my::vk_structure_chain<my::vk_instance_createinfo, my::vk_dbg_utl_msgr_createinfo_ext> createInfo {
         {{}, &info, validationlayers, extensions},
         my::debug_utl::messenger_create_info()
      };

      return vk::createInstanceUnique( createInfo.get<my::vk_instance_createinfo>(), nullptr, dld );
   }

   my::unq_vk_instance
   create( const my::vk_applicationinfo& info, const my::vk_dispatchloader_dynamic& dld )
   {
      const auto extensions { my::ext::get_required_extensions( dld ) };

      const my::vk_instance_createinfo createInfo { {}, &info, {}, extensions };

      return vk::createInstanceUnique( createInfo, nullptr, dld );
   }
}    // namespace
namespace my_library
{
   void
   vulkan::instance::init( const std::string app_name, const bool debug )
   {

      const vk_applicationinfo app_info( app_name.c_str(),
                                         VK_MAKE_VERSION( 1, 0, 0 ),
                                         app_name.c_str(),
                                         VK_MAKE_VERSION( 1, 0, 0 ),
                                         VK_API_VERSION_1_2 );

      if ( const shared_container c = ctr_lock() )
      {
         // vk_instanceを作成し、コンテナに登録
         c->register_ctr( debug ? create_dbg( app_info, c->dld ) : create( app_info, c->dld ) );
         utl::log( "create instance succeeded." );
         utl::log( "register vk_instance to container succeeded." );

         if ( debug )
         {
            _vk_dbg_magr =
              c->instance->createDebugUtilsMessengerEXTUnique( debug_utl::messenger_create_info(), nullptr, c->dld );
            utl::log( "DebugUtils Messenger setup completed." );
         }
      }
   }

   vulkan::instance::instance( const shared_container& c ) : vkobject { c } {}

   vulkan::instance::~instance() {}
}    // namespace my_library
