#include "instance.hpp"

#include <GLFW/glfw3.h>    //拡張機能を取得するために必要
#include <iostream>
#include <string>
namespace
{
   const std::vector<const char*> validationlayers = { "VK_LAYER_KHRONOS_validation" };
#ifdef NDEBUG
   const bool enable_validationlayers = false;
#else
   const bool enable_validationlayers = true;
#endif    // NDEBUG
}
namespace my_library
{
   vulkan::instance*
   vulkan::instance::create()
   {
      return new create_helper();
   }
   vulkan::instance::instance() {}
   std::vector<const char*>
   vulkan::instance::get_required_extensions( const bool enable_validationlayers )
   {
      uint32_t     glfw_extension_count = 0;
      const char** glfw_extensions;
      glfw_extensions = glfwGetRequiredInstanceExtensions( &glfw_extension_count );
      std::vector<const char*> extensions( glfw_extensions, glfw_extensions + glfw_extension_count );
      // MacOsに対応するため入れる
      extensions.emplace_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );
      // デバッグ時有効
      if ( enable_validationlayers ) { extensions.emplace_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME ); }
      return extensions;
   }

   bool
   vulkan::instance::check_extension_support( const std::vector<const char*>& extensions )
   {
      // 全てのサポートしている拡張機能の数のみ取得
      std::uint32_t extension_count = 0;
      vkEnumerateInstanceExtensionProperties( nullptr, &extension_count, nullptr );

      // 拡張機能の情報を取得
      std::vector<VkExtensionProperties> available_extensions( extension_count );
      vkEnumerateInstanceExtensionProperties( nullptr, &extension_count, available_extensions.data() );

      // 拡張機能の名前を一つにまとめる

      // 拡張機能がサポートされているか一覧からチェック
      for ( const char* extension : extensions )
      {
         bool extension_found = false;
         for ( const auto& extension_property : available_extensions )
         {
            if ( std::equal( std::string_view { extension_property.extensionName }.begin(),
                             std::string_view { extension_property.extensionName }.end(),
                             std::string_view { extension }.begin(),
                             std::string_view { extension }.end() ) )
            {
               std::cout << extension << " is support" << std::endl;
               extension_found = true;
               break;
            }
         }
         if ( !extension_found ) return false;
      }
      return true;
   }

   bool
   vulkan::instance::check_validationlayer_support( const std::vector<const char*>& validationlayers )
   {
      // 使用可能なレイヤー数を取得
      std::uint32_t layer_count;
      vkEnumerateInstanceLayerProperties( &layer_count, nullptr );
      // 使用可能レイヤーの情報を取得
      std::vector<VkLayerProperties> availablelayers( layer_count );
      vkEnumerateInstanceLayerProperties( &layer_count, availablelayers.data() );
      // 使用したいレイヤーが全てサポートされているか確認
      for ( const char* layer_name : validationlayers )
      {
         bool layer_found = false;
         for ( const auto& layer_propertys : availablelayers )
         {
            if ( std::equal( std::string_view { layer_propertys.layerName }.begin(),
                             std::string_view { layer_propertys.layerName }.end(),
                             std::string_view { layer_name }.begin(),
                             std::string_view { layer_name }.end() ) )
            {
               layer_found = true;
               break;
            }
         }
         if ( !layer_found ) return false;
      }
      return true;
   }
}    // namespace my_library