#include "vulkan.hpp"

#include <GLFW/glfw3.h>    //拡張機能を取得するために必要
#include <algorithm>
#include <iostream>
#include <string>

namespace
{
   const std::uint32_t            WIDTH            = 800;
   const std::uint32_t            HEIGHT           = 600;
   const std::vector<const char*> validationlayers = { "VK_LAYER_KHRONOS_validation" };
#ifdef NDEBUG
   const bool enable_validationlayers = false;
#else
   const bool enable_validationlayers = true;
#endif         // NDEBUG

   VkResult    // VkDebugUtilsMessengerEXTを作成する関数を探し出す
   create_debug_utils_messenger_ext( VkInstance                                instance,
                                     const VkDebugUtilsMessengerCreateInfoEXT* create_info_ptr,
                                     const VkAllocationCallbacks*              allocator_ptr,
                                     VkDebugUtilsMessengerEXT*                 debug_messenger_ptr )
   {
      auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT" ) );
      if ( func != nullptr ) { return func( instance, create_info_ptr, allocator_ptr, debug_messenger_ptr ); }
      else { return VK_ERROR_EXTENSION_NOT_PRESENT; }
   }
   void    // VkDebugUtilsMessengerEXTを解放する関数を探し出す
   destroy_debug_utils_messenger_ext( VkInstance                   instance,
                                      VkDebugUtilsMessengerEXT     debug_messenger,
                                      const VkAllocationCallbacks* allocator_ptr )
   {
      auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr( instance, "vkDestroyDebugUtilsMessengerEXT" ) );
      if ( func != nullptr ) { func( instance, debug_messenger, allocator_ptr ); }
   }

}    // namespace

namespace lib::renderer
{
   vulkan::vulkan() : vk_instance { VK_NULL_HANDLE }, vk_debug_messenger { VK_NULL_HANDLE } {}
   /*
    VkDebugUtilsMessageSeverityFlagBitsEXT :　メッセージの重要度
    VkDebugUtilsMessageTypeFlagsEXT : メッセージの種類
    const VkDebugUtilsMessageCallbackDataEXT* : メッセージの詳細を含む構造体、pCallbackDataを参照。
   */
   VKAPI_ATTR VkBool32 VKAPI_CALL
   vulkan::debag_callBack( VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
                           VkDebugUtilsMessageTypeFlagsEXT             message_type,
                           const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
                           void*                                       user_data_ptr )
   {
      std::cerr << "validation layer:" << callback_data_ptr->pMessage << std::endl;
      return VK_FALSE;
   }

   void
   vulkan::create_instace()
   {
      if ( enable_validationlayers
           && !check_validationlayer_support( validationlayers ) )    // デバッグ時のみ有効ニスル
      {
         throw std::runtime_error( "validation layers requested, but not available!" );
      }

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

      // インスタンス作成の詳細設定
      VkInstanceCreateInfo create_info {};
      create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      create_info.pApplicationInfo = &app_info;
      // mac os に対応するためこのフラグがいる
      create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

      // ==[ 拡張機能 ]==========================
      std::vector<const char*> extensions { get_required_extensions( enable_validationlayers ) };
      if ( !check_extension_support( extensions ) )
      {
         throw std::runtime_error( "extensions requested, but not available!" );
      }
      create_info.enabledExtensionCount   = static_cast<uint32_t>( extensions.size() );
      create_info.ppEnabledExtensionNames = extensions.data();

      // ==[ 検証レイヤー ]===========================
      //vkCreateInctance, vkDestroyInstanceの内部イベントに対応するためもう一つのdebugMessengerを用意する。
      VkDebugUtilsMessengerCreateInfoEXT debug_create_info {};
      if ( enable_validationlayers )    // デバッグ時のみ有効ニスル
      {
         create_info.enabledLayerCount   = static_cast<uint32_t>( validationlayers.size() );
         create_info.ppEnabledLayerNames = validationlayers.data();
         populate_debug_messenger_create_info( debug_create_info );
         //ここでpNextにdebug_create_infoを指定することでcreate_infoが拡張されデバッグできるようになる。
         create_info.pNext = reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT*>( &debug_create_info );
      }
      else
      {
         create_info.enabledLayerCount = 0;
         create_info.pNext             = nullptr;
      }

      // ==[ ようやく生成 ]=============================
      if ( vkCreateInstance( &create_info, nullptr, &vk_instance ) != VK_SUCCESS )
      {
         throw std::runtime_error( "failed to create instance!" );
      }
   }
   void
   vulkan::setup_debug_messenger()
   {
      if ( !enable_validationlayers ) return;

      VkDebugUtilsMessengerCreateInfoEXT create_info {};
      populate_debug_messenger_create_info( create_info );

      if ( create_debug_utils_messenger_ext( vk_instance, &create_info, nullptr, &vk_debug_messenger ) != VK_SUCCESS )
      {
         throw std::runtime_error( "failed to set up debug messenger!" );
      }
   }

   std::vector<const char*>
   vulkan::get_required_extensions( const bool enable_validationlayers )
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
   vulkan::check_validationlayer_support( const std::vector<const char*>& validationlayers )
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
   bool
   vulkan::check_extension_support( const std::vector<const char*>& extensions )
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
   void
   vulkan::populate_debug_messenger_create_info( VkDebugUtilsMessengerCreateInfoEXT& create_info )
   {
      create_info                 = {};
      create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT    // より詳細な情報
                                  | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT    // 警告
                                  | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;     // エラー
      create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    // いったんすべての種類を表示
                              | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                              | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      create_info.pfnUserCallback = debag_callBack;
      create_info.pUserData       = nullptr;
   }
   void
   vulkan::init()
   {
      create_instace();
      setup_debug_messenger();
   }
   void
   vulkan::release()
   {
      if ( enable_validationlayers ) { destroy_debug_utils_messenger_ext( vk_instance, vk_debug_messenger, nullptr ); }
      vkDestroyInstance( vk_instance, nullptr );
      delete this;
   }
}    // namespace lib::renderer

namespace api::renderer
{
   vulkan*
   create()
   {
      return new lib::renderer::vulkan();
   }
}    // namespace api::renderer