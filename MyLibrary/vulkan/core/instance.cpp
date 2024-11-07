#include "instance.hpp"

#include "../../pch.hpp"
#include "../../utilities.hpp"
#include "debugUtils.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
namespace
{
   namespace vkm               = my_library::vkm;
   namespace utl               = my_library::utl;
   using UniqueInstance        = vkm::instance::UniqueHandle;
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
   create( const std::string&              app_name,
           const std::vector<const char*>& extensions,
           const std::vector<const char*>& validationlayers,
           const DispatchLoaderDynamic&    dld )
   {
      const vkm::ApplicationInfo app_info( app_name.c_str(),
                                           VK_MAKE_VERSION( 1, 0, 0 ),
                                           app_name.c_str(),
                                           VK_MAKE_VERSION( 1, 0, 0 ),
                                           VK_API_VERSION_1_2 );

      assert( checkExtensionSupport( extensions, dld ) );
      utl::log( "instance extensions is supported." );
#ifdef _DEBUG
      assert( !validationlayers.empty() );
      const vkm::instance::CreateInfo create_info { {}, &app_info, validationlayers, extensions };
      UniqueInstance                  instance { vk::createInstanceUnique( create_info, nullptr, dld ) };
#else
      const vkm::instance::CreateInfo createInfo { {}, &app_info, {}, extensions };
      UniqueInstance                  instance { vk::createInstanceUnique( createInfo, nullptr, dld ) };
#endif
      utl::runtimeError( instance->operator bool(), "failed create instance!" );
      return instance;
   }
}    // namespace
namespace my_library
{
   namespace vkm
   {
      void
      Instance::create( const std::string&              app_name,
                        const std::vector<const char*>& extensions,
                        const std::vector<const char*>& validationlayers )
      {

         const vkm::ApplicationInfo app_info( app_name.c_str(),
                                              VK_MAKE_VERSION( 1, 0, 0 ),
                                              app_name.c_str(),
                                              VK_MAKE_VERSION( 1, 0, 0 ),
                                              VK_API_VERSION_1_2 );

         assert( checkExtensionSupport( extensions, dld ) );
         utl::log( "instance extensions is supported." );

#ifdef _DEBUG
         assert( !validationlayers.empty() );
         const vkm::instance::CreateInfo create_info { {}, &app_info, validationlayers, extensions };
#else
         const vkm::instance::CreateInfo create_info { {}, &app_info, {}, extensions };
#endif

         _instance = vk::createInstanceUnique( create_info, nullptr, dld );
         utl::runtimeError( _instance->operator bool(), "failed create instance!" );
         utl::log( "create instance succeeded." );

         // 関数ポインタ取得
         _dld.init( *_instance );
         utl::log( "get function pointer to dispatch loader dynamic." );
      }

      const std::vector<device::Gpu>
      Instance::enumeratePhysicalDevices()
      {
         return _instance->enumeratePhysicalDevices( _dld );
      }

      Instance::Instance() : dld { _dld }, vk_instance { _instance } { _dld.init(); }
      Instance::~Instance() {}
   }    // namespace vkm

}    // namespace my_library
