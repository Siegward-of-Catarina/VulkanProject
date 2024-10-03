#include "instance.hpp"

#include "../../../pch.hpp"
#include "../../utilities.hpp"
#include "../debugUtils.hpp"
#include "../queuefamily.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
namespace
{
   namespace vkm               = my_library::vkm;
   using UniqueInstance        = vkm::UniqueInstance;
   using InstanceCreateDatas   = vkm::instance::CreateInfo;
   using DispatchLoaderDynamic = vkm::DispatchLoaderDynamic;

   const bool
   checkExtensionSupport( std::vector<const char*> extensions, const DispatchLoaderDynamic& dld )
   {
      // 拡張機能の情報を取得
      std::vector<vkm::ExtensionProperties> available_extensions { vk::enumerateInstanceExtensionProperties( nullptr,
                                                                                                             dld ) };
      // 拡張機能がサポートされているか一覧からチェック
      for ( const char* extension : extensions )
      {
         bool extension_found { false };
         for ( const auto& extension_property : available_extensions )
         {
            if ( strcmp( extension, extension_property.extensionName ) == 0 )
            {
               extension_found = true;
               break;
            }
         }
         if ( !extension_found ) return false;
      }
      return true;
   }

   UniqueInstance
   create( const InstanceCreateDatas& create_datas, const DispatchLoaderDynamic& dld )
   {
      const vkm::ApplicationInfo app_info( create_datas.app_name.c_str(),
                                           VK_MAKE_VERSION( 1, 0, 0 ),
                                           create_datas.app_name.c_str(),
                                           VK_MAKE_VERSION( 1, 0, 0 ),
                                           VK_API_VERSION_1_2 );

      assert( checkExtensionSupport( create_datas.extensions, dld ) );
      my_library::utl::log( "instance extensions is supported." );

      const vkm::InstanceCreateInfo createInfo { {}, &app_info, {}, create_datas.extensions };
      UniqueInstance                instance { vk::createInstanceUnique( createInfo, nullptr, dld ) };
      if ( !instance ) { my_library::utl::runtimeError( "failed create instance." ); }
      return instance;
   }
   UniqueInstance
   createDebug( const InstanceCreateDatas& create_datas, const DispatchLoaderDynamic& dld )
   {
      const vk::ApplicationInfo app_info( create_datas.app_name.c_str(),
                                          VK_MAKE_VERSION( 1, 0, 0 ),
                                          create_datas.app_name.c_str(),
                                          VK_MAKE_VERSION( 1, 0, 0 ),
                                          VK_API_VERSION_1_2 );

      assert( checkExtensionSupport( create_datas.extensions, dld ) );
      my_library::utl::log( "instance extensions is supported." );

      assert( !create_datas.validationlayers.empty() );

      const vkm::StructureChain<vkm::InstanceCreateInfo, vkm::DebugUtilsMessengerCreateInfoEXT> create_instance_info {
         {{}, &app_info, create_datas.validationlayers, create_datas.extensions},
         create_datas.debug_info
      };

      UniqueInstance instance =
        vk::createInstanceUnique( create_instance_info.get<vkm::InstanceCreateInfo>(), nullptr, dld );
      if ( !instance ) { my_library::utl::runtimeError( "failed create instance." ); }
      my_library::utl::log( "create vkInstance succeeded." );

      return instance;
   }
}    // namespace
namespace my_library
{
   namespace vkm
   {
      void
      Instance::create( const instance::CreateInfo& create_info, const bool debug )
      {
         if ( debug )
         {
            _instance = std::move( ::createDebug( create_info, _dld ) );
            // 関数ポインタ取得
            _dld.init( *_instance );
            _dbg_messenger = _instance->createDebugUtilsMessengerEXTUnique( create_info.debug_info, nullptr, _dld );
         }
         else
         {
            _instance = std::move( ::create( create_info, _dld ) );
            // 関数ポインタ取得
            _dld.init( *_instance );
         }

         for ( auto& device : _instance->enumeratePhysicalDevices( _dld ) ) { _devices.emplace_back( device, 0u ); }
      }

      const std::vector<PhysicalDevice>
      Instance::enumeratePhysicalDevices()
      {
         return _instance->enumeratePhysicalDevices( _dld );
      }

      Instance::Instance() : dld { _dld }, vkobj { _instance }, _devices {}
      {
         _dld.init();
      }
      Instance::~Instance() {}
   }    // namespace vkm

}    // namespace my_library
