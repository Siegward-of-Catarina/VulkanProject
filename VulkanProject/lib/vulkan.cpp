#include "vulkan.hpp"

#include <GLFW/glfw3.h>    //拡張機能を取得するために必要
#include <algorithm>
#include <iostream>
#include <string>

namespace
{
   const std::uint32_t WIDTH  = 800;
   const std::uint32_t HEIGHT = 600;
}    // namespace

namespace lib::renderer
{

   vulkan::vulkan() : vk_instance { VK_NULL_HANDLE } {}
   void vulkan::create_instace()
   {
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

      VkInstanceCreateInfo create_info {};
#ifdef NDEBUG
      const bool enable_validationlayers = false;
#else
      const bool enable_validationlayers = true;
#endif    // NDEBUG
      // 検証レイヤー
      std::vector<const char*> validationlayers { "VK_LAYER_KHRONOS_validation" };

      // インスタンス作成の詳細設定
      create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      create_info.pApplicationInfo = &app_info;
      // mac os に対応するためこのフラグがいる
      create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
      // ==[ 拡張機能 ]==========================
      auto extensions = get_required_extensions( enable_validationlayers );
      if ( !check_extension_support( extensions ) )
         throw std::runtime_error( "extensions requested, but not available!" );

      create_info.enabledExtensionCount   = static_cast<uint32_t>( extensions.size() );
      create_info.ppEnabledExtensionNames = extensions.data();
      // ==[ 検証レイヤー ]================================
      if ( enable_validationlayers )
      {
         if ( !check_validationlayer_support( validationlayers ) )
            throw std::runtime_error( "validation layers requested, but not available!" );

         create_info.enabledLayerCount   = static_cast<uint32_t>( validationlayers.size() );
         create_info.ppEnabledLayerNames = validationlayers.data();
      }
      else { create_info.enabledLayerCount = 0; }
      //get_vkinstance_create_info( &app_info, &create_info );

      // ようやく生成
      if ( vkCreateInstance( &create_info, nullptr, &vk_instance ) != VK_SUCCESS )
      {
         throw std::runtime_error( "failed to create instance!" );
      }
   }
   void vulkan::get_vkinstance_create_info( VkApplicationInfo* app_info, VkInstanceCreateInfo* create_info )
   {

#ifdef NDEBUG
      const bool enable_validationlayers = false;
#else
      const bool enable_validationlayers = true;
#endif    // NDEBUG
      // 検証レイヤー
      std::vector<const char*> validationlayers { "VK_LAYER_KHRONOS_validation" };

      // インスタンス作成の詳細設定
      create_info->sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      create_info->pApplicationInfo = app_info;
      // mac os に対応するためこのフラグがいる
      create_info->flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
      // ==[ 拡張機能 ]==========================
      auto extensions = get_required_extensions( enable_validationlayers );
      if ( !check_extension_support( extensions ) )
         throw std::runtime_error( "extensions requested, but not available!" );

      create_info->enabledExtensionCount   = static_cast<uint32_t>( extensions.size() );
      create_info->ppEnabledExtensionNames = extensions.data();
      // ==[ 検証レイヤー ]================================
      if ( enable_validationlayers )
      {
         if ( !check_validationlayer_support( validationlayers ) )
            throw std::runtime_error( "validation layers requested, but not available!" );

         create_info->enabledLayerCount   = static_cast<uint32_t>( validationlayers.size() );
         create_info->ppEnabledLayerNames = validationlayers.data();
      }
      else { create_info->enabledLayerCount = 0; }
   }

   std::vector<const char*> vulkan::get_required_extensions( const bool enable_validationlayers )
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

   bool vulkan::check_validationlayer_support( std::vector<const char*> validationlayers )
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
   bool vulkan::check_extension_support( std::vector<const char*> extensions )
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

   void vulkan::init() { create_instace(); }
   void vulkan::release()
   {
      vkDestroyInstance( vk_instance, nullptr );
      delete this;
   }
}    // namespace lib::renderer

namespace api::renderer
{
   vulkan* create() { return new lib::renderer::vulkan(); }
}    // namespace api::renderer