#include "vulkan.hpp"

#include <GLFW/glfw3.h>    //拡張機能を取得するために必要
#include <algorithm>
#include <string>
#include <vector>

namespace
{
   const std::uint32_t WIDTH  = 800;
   const std::uint32_t HEIGHT = 600;
}    // namespace


namespace lib::renderer
{
   void vulkan::create_instace()
   {
       //検証レイヤー
      const std::vector<const char*> validationlayers { "VK_LAYER_KHRONOS_validation" };
      #ifdef NDEBUG
      const bool enable_validationlayers = false;
      #else
      const bool enable_validationlayers = true;
      #endif



      VkApplicationInfo app_info {};
      // アプリケーションの詳細設定
       {
         app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
         app_info.pApplicationName   = "Hello Triangle!";
         app_info.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
         app_info.pEngineName        = "No Engine";
         app_info.engineVersion      = VK_MAKE_VERSION( 1, 0, 0 );
         app_info.apiVersion         = VK_API_VERSION_1_0;
       }

      // ウィンドウシステムの拡張機能を取得
      std::uint32_t glfw_extension_count = 0;
      const char**  glfw_extensions;
      glfw_extensions = glfwGetRequiredInstanceExtensions( &glfw_extension_count );

      // インスタンス作成の詳細設定
      VkInstanceCreateInfo create_info {};
      {
         create_info.sType             = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
         create_info.pApplicationInfo  = &app_info;
         create_info.enabledLayerCount = 0;    // 今は空
                                               // create_info.ppEnabledLayerNames

         // OSによって拡張機能の追加が生じるので分岐
#if defined( _WIN32 )
         create_info.enabledExtensionCount   = glfw_extension_count;
         create_info.ppEnabledExtensionNames = glfw_extensions;
#elif defined( __APPLE__ )    // macos
         std::vector<const char*> required_extensions;
         for ( std::uint32_t i = 0; i < glfw_extension_count; i++ )
         {
            required_extensions.emplace_back( glfw_extensions[ i ] );
         }

         required_extensions.emplace_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );

         create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

         create_info.enabledExtensionCount   = static_cast<uint32_t>( required_extensions.size() );
         create_info.ppEnabledExtensionNames = required_extensions.data();
#endif
      }
      
      // 拡張機能がサポートされているか確認
      check_extension_support( glfw_extensions, glfw_extension_count );

      // ようやく生成
      if ( vkCreateInstance( &create_info, nullptr, &vk_instance ) != VK_SUCCESS )
      {
         throw std::runtime_error( "failed to create instance!" );
      }

   }

   bool vulkan::check_validationlayer_support( std::vector<const char*, std::allocator<const char*> > validationlayers )
   { 
       //使用可能なレイヤー数を取得
       std::uint32_t layer_count;
      vkEnumerateInstanceLayerProperties( &layer_count, nullptr );
      //使用可能レイヤーの情報を取得
       std::vector<VkLayerProperties> availablelayers( layer_count );
      vkEnumerateInstanceLayerProperties( &layer_count, availablelayers.data() );
      //使用したいレイヤーが全てサポートされているか確認
      for ( const char* layerName : validationlayers ) {}
      return false;
   }

   void vulkan::check_extension_support( const char** extensions, const std::uint32_t count )
   {
      // 全てのサポートしている拡張機能の数のみ取得
      std::uint32_t extension_count = 0;
      vkEnumerateInstanceExtensionProperties( nullptr, &extension_count, nullptr );

      // 拡張機能の情報を取得
      std::vector<VkExtensionProperties> supported_extensions( extension_count );
      vkEnumerateInstanceExtensionProperties( nullptr, &extension_count, supported_extensions.data() );

      // 拡張機能の名前を一つにまとめる
      std::string supported_extension_names;
      for ( const auto& extension : supported_extensions )
      {
         supported_extension_names += extension.extensionName;
         supported_extension_names += "\n";
      }

      // 拡張機能がサポートされているか一覧からチェック
      for ( std::uint32_t i = 0; i < count; i++ )
      {
         if ( supported_extension_names.find( extensions[ i ] ) != std::string::npos )
            std::cout << extensions[ i ] << " is supported" << std::endl;
         else throw std::runtime_error( std::string { extensions[ i ] } + std::string { " is not supported" } );
      }
   }
   void vulkan::init() { create_instace(); }
   void vulkan::release()
   {
      vkDestroyInstance( vk_instance, nullptr );
      delete this;
   }
   vulkan::vulkan() : vk_instance { VK_NULL_HANDLE } {}
}    // namespace lib::renderer


namespace api::renderer
{
   vulkan* create() { return new lib::renderer::vulkan(); }
}    // namespace api::renderer