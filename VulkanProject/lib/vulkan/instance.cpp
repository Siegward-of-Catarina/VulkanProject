#include "instance.hpp"

#include <GLFW/glfw3.h>    //拡張機能を取得するために必要
#include <iostream>
#include <string>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace
{
   // extensions ------------------------------------------------
   bool
   check_extension_support( const std::vector<const char*>& extensions, const vk::DispatchLoaderDynamic& dld )
   {
      // 拡張機能の情報を取得
      std::vector<vk::ExtensionProperties> available_extensions { vk::enumerateInstanceExtensionProperties( nullptr,
                                                                                                            dld ) };

      // 拡張機能がサポートされているか一覧からチェック
      for ( const char* extension : extensions )
      {
         bool extension_found = false;
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

   std::vector<const char*>
   get_required_extensions( const vk::DispatchLoaderDynamic& dld )
   {
      uint32_t     glfw_extension_count = 0;
      const char** glfw_extensions;
      glfw_extensions = glfwGetRequiredInstanceExtensions( &glfw_extension_count );
      std::vector<const char*> extensions { glfw_extensions, glfw_extensions + glfw_extension_count };

#ifdef __APPLE__    // MacOsに対応するため入れる
      extensions.emplace_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );
#endif
#ifdef _DEBUG
      extensions.emplace_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
#endif

      if ( !check_extension_support( extensions, dld ) )
      {
         throw std::runtime_error( "extensions requested, but not available!" );
      }

      return extensions;
   }

}    // namespace

namespace my_library
{
   void
   vulkan::instance::init( std::string                                 app_name,
                           const std::vector<const char*>&             validationlayers,
                           const vk::DebugUtilsMessengerCreateInfoEXT& d_info,
                           vk::DispatchLoaderDynamic&                  dld )
   {
      vk::ApplicationInfo appInfo(
        "Hello Triangle", VK_MAKE_VERSION( 1, 0, 0 ), "No Engine", VK_MAKE_VERSION( 1, 0, 0 ), VK_API_VERSION_1_2 );

      auto extensions = get_required_extensions( dld );

      vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT> createInfo(
        { {}, &appInfo, validationlayers, extensions }, d_info );

      _vk_instance = vk::createInstanceUnique( createInfo.get<vk::InstanceCreateInfo>(), nullptr, dld );

      // 全ての関数ポインタを取得する
      dld.init( *_vk_instance );
   }

   void
   vulkan::instance::init( std::string app_name, vk::DispatchLoaderDynamic& dld )
   {
      vk::ApplicationInfo appInfo(
        "Hello Triangle", VK_MAKE_VERSION( 1, 0, 0 ), "No Engine", VK_MAKE_VERSION( 1, 0, 0 ), VK_API_VERSION_1_2 );

      auto extensions = get_required_extensions( dld );

      // in non-debug mode
      vk::InstanceCreateInfo createInfo( {}, &appInfo, {}, extensions );
      _vk_instance = vk::createInstanceUnique( createInfo, nullptr, dld );

      // 全ての関数ポインタを取得する
      dld.init( *_vk_instance );
   }

   vulkan::unq_vk_instance&
   vulkan::instance::vk_instance()
   {
      return _vk_instance;
   }

   vulkan::instance::instance() : _vk_instance {} {}

   vulkan::instance::~instance() {}
}    // namespace my_library
