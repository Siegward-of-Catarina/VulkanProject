#include "vulkan.hpp"

#include "../glwindow.hpp"
#include "debug_messenger.hpp"
#include "extensions.hpp"

#include <GLFW/glfw3.h>    //拡張機能を取得するために必要
#include <algorithm>
#include <iostream>
#include <map>
#include <string>

namespace
{
   const std::uint32_t WIDTH  = 800;
   const std::uint32_t HEIGHT = 600;
#ifdef NDEBUG
   const std::vector<const char*> validationlayers;
#else
   const std::vector<const char*> validationlayers { "VK_LAYER_KHRONOS_validation" };
#endif    // NDEBUG

}    // namespace

namespace my_library
{
   // vulkan::core
   namespace vulkan
   {
      vulkan::vulkan()
        : vk_instance { VK_NULL_HANDLE }
        , vk_debug_messenger { VK_NULL_HANDLE }
        , vk_physical_device { VK_NULL_HANDLE }
        , debug_messenger { debug_messenger::create() }
      {}

      vulkan*
      vulkan::create()
      {
         return new create_helper();
      }

      void
      vulkan::create_instace()
      {
         if ( !validationlayers.empty()
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
         create_info.sType             = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
         create_info.pApplicationInfo  = &app_info;
         create_info.enabledLayerCount = 0;          // いったんこれで設定しておく
         create_info.pNext             = nullptr;    // いったんこれで設定しておく
         // mac os に対応するためこのフラグがいる
         create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

         // ==[ 拡張機能 ]==========================
         extensions::register_for_create_info( create_info );

         // ==[ 検証レイヤー ]===========================
         if ( !validationlayers.empty() ) debug_messenger->register_for_create_info( create_info, validationlayers );

         // ==[ ようやく生成 ]=============================
         if ( vkCreateInstance( &create_info, nullptr, &vk_instance ) != VK_SUCCESS )
         {
            throw std::runtime_error( "failed to create instance!" );
         }
      }

      void
      vulkan::pick_up_physical_device()
      {
         // GPUの個数をまず取得
         uint32_t device_count = 0;
         vkEnumeratePhysicalDevices( vk_instance, &device_count, nullptr );
         if ( device_count == 0 ) { throw std::runtime_error( "failed to find GPUs with Vulkan support!" ); }
         // GPUのリストを取得
         std::vector<VkPhysicalDevice> devices( device_count );
         vkEnumeratePhysicalDevices( vk_instance, &device_count, devices.data() );

         std::multimap<int, VkPhysicalDevice> candidates;

         // GPUのリストから適切なデバイスを取得
         for ( const auto& device : devices )
         {
            int score = rate_device_suitability( device );
            candidates.insert( std::make_pair( score, device ) );

            if ( is_device_suitable( device ) )
            {
               vk_physical_device = device;
               break;
            }
         }
         // rbegin 末尾　-> 先頭　の逆イテレータ
         if ( candidates.rbegin()->first > 0 ) { vk_physical_device = candidates.rbegin()->second; }
         else { throw std::runtime_error( "failed to find a suitable GPU!" ); }
      }

      void
      vulkan::create_logical_device()
      {
         // 論理デバイスを作成するためにキューの詳細を決める
         queue_family_indices indices =
           find_queue_familie( vk_physical_device );    // 今はグラフィック機能を備えたキューがあればいい
         float queue_priority = 1.0f;    // キューに対して優先度をつけるための値 //現状キューは1つだが、必要
         VkDeviceQueueCreateInfo queue_create_info {};
         queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
         queue_create_info.queueFamilyIndex = indices.graphics_family.value();
         queue_create_info.queueCount       = 1;
         queue_create_info.pQueuePriorities = &queue_priority;

         // デバイスの機能セットを指定する
         VkPhysicalDeviceFeatures device_features {};

         // 論理デバイスの詳細を設定
         VkDeviceCreateInfo create_info {};
         create_info.sType                 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
         create_info.pQueueCreateInfos     = &queue_create_info;
         create_info.queueCreateInfoCount  = queue_create_info.queueCount;
         create_info.pEnabledFeatures      = &device_features;
         create_info.enabledExtensionCount = 0;
         // デバイス固有の拡張機能が対応しているかチェックする

         // デバッグ時のみ検証レイヤーを有効化
         if ( !validationlayers.empty() )
         {
            create_info.enabledLayerCount   = static_cast<uint32_t>( validationlayers.size() );
            create_info.ppEnabledLayerNames = validationlayers.data();
         }
         else { create_info.enabledLayerCount = 0; }

         // 論理デバイスを作成
         if ( vkCreateDevice( vk_physical_device, &create_info, nullptr, &vk_device ) != VK_SUCCESS )
         {
            throw std::runtime_error( "faied to create logical device!" );
         }

         // キューをハンドルに取得
         vkGetDeviceQueue( vk_device, indices.graphics_family.value(), 0, &vk_graphics_queue );
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
      bool
      vulkan::is_device_suitable( VkPhysicalDevice device )
      {
         queue_family_indices indices = find_queue_familie( device );

         return indices.isComplete();
      }
      int
      vulkan::rate_device_suitability( VkPhysicalDevice device )
      {
         int score = 0;
         // デバイスの基本情報を取得
         VkPhysicalDeviceProperties device_properties;
         vkGetPhysicalDeviceProperties( device, &device_properties );
         // デバイスのオプション情報を取得
         VkPhysicalDeviceFeatures device_features;
         vkGetPhysicalDeviceFeatures( device, &device_features );

         // 個別のGPUが積まれているかどうか(CPU内臓グラフィックでない)
         if ( device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ) { score += 1000; }

         // 最大画像サイズ
         score += device_properties.limits.maxImageDimension2D;

         // ジオメトリシェーダーへの対応は必須とする
         if ( !device_features.geometryShader ) { return 0; }
         if ( !is_device_suitable( device ) ) { return 0; }

         return score;
      }
      queue_family_indices
      vulkan::find_queue_familie( VkPhysicalDevice device )
      {
         queue_family_indices indices;

         // キューファミリーのリストを取得
         uint32_t family_count = 0;
         vkGetPhysicalDeviceQueueFamilyProperties( device, &family_count, nullptr );

         std::vector<VkQueueFamilyProperties> queue_families( family_count );
         vkGetPhysicalDeviceQueueFamilyProperties( device, &family_count, queue_families.data() );

         int i = 0;
         for ( const auto& queue_family : queue_families )
         {
            if ( queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT ) { indices.graphics_family = i; }
            if ( indices.isComplete() ) { break; }
            i++;
         }
         return indices;
      }
      void
      vulkan::init( const my_library::window::glwindow& window )
      {
         create_instace();
         if ( !validationlayers.empty() ) debug_messenger->setup_messenger( vk_instance );
         pick_up_physical_device();
      }
      void
      vulkan::release()
      {
         // vkDestroyDevice( vk_device, nullptr );    // 論理デバイス
         if ( !validationlayers.empty() )    // デバッグメッセンジャー
         {
            debug_messenger->release( vk_instance );
         }
         vkDestroyInstance( vk_instance, nullptr );    // Vulkanインスタンス
         delete this;                                  // デストラクタ
      }
   }                                                   // namespace vulkan
   // vulkan::queue_family
   namespace vulkan
   {
      bool
      queue_family_indices::isComplete()
      {
         return graphics_family.has_value();
      }
   }    // namespace vulkan

}    // namespace my_library
