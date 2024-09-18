#include "extension.hpp"

#include "../utilities.hpp"

#include <GLFW/glfw3.h>    //拡張機能を取得するために必要
#include <set>
namespace my_library::vkm::ext
{

   const bool
   checkExtensionSupport( const std::vector<const char*>&               extensions,
                          const my_library::vkm::DispatchLoaderDynamic& dld )
   {
      // 拡張機能の情報を取得
      std::vector<my_library::vkm::ExtensionProperties> available_extensions { vk::enumerateInstanceExtensionProperties(
        nullptr, dld ) };
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

   const bool
   check_device_ext_support( const PhysicalDevice& device )
   {
      const std::vector<my_library::vkm::ExtensionProperties> available_exts {
         device.enumerateDeviceExtensionProperties()
      };

      const auto&           dev_ext { get_device_exts() };
      std::set<std::string> required_exts { dev_ext.begin(), dev_ext.end() };
      // 要求された拡張機能配列から有効な拡張機能が見つかれば削除していく。
      for ( const auto& ext : available_exts ) { required_exts.erase( ext.extensionName ); }

      my_library::utl::log( "requested device extensions is a suported." );

      // すべて有効だった場合 empty になる
      return required_exts.empty();
   }

   const std::vector<const char*>
   get_device_exts()
   {
      return std::vector<const char*> { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
   }
}    // namespace my_library::vulkan::ext