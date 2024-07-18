#include "vulkan.hpp"

#include "instance.hpp"
#include "vulkan_debug.hpp"

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
      void
      vulkan::pick_up_physical_device()
      {
#if 0
         // GPUの個数をまず取得
         uint32_t                    device_count = 0;
         std::shared_ptr<VkInstance> vk_instance { _instance->get_vk_instance() };
         vkEnumeratePhysicalDevices( *( vk_instance.get() ), &device_count, nullptr );
         if ( device_count == 0 ) { throw std::runtime_error( "failed to find GPUs with Vulkan support!" ); }
         // GPUのリストを取得
         std::vector<VkPhysicalDevice> devices( device_count );
         vkEnumeratePhysicalDevices( *vk_instance, &device_count, devices.data() );

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
#endif
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

      bool
      vulkan::check_validationlayer_support()
      {
         std::vector<vk::LayerProperties> availablelayers = vk::enumerateInstanceLayerProperties();

         for ( const char* layername : validationlayers )
         {
            bool layerfound = false;
            for ( const auto& layerproperties : availablelayers )
            {
               if ( strcmp( layername, layerproperties.layerName ) == 0 )
               {
                  layerfound = true;
                  break;
               }
            }
            if ( !layerfound ) { return false; }
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
      vulkan::init( const GLFWwindow* window )
      {

         dld.init();

         if ( !validationlayers.empty() && !check_validationlayer_support() )
         {
            throw std::runtime_error( "validation layers requested, but not available!" );
         }

         _instance->init( "hello triangle", validationlayers, _vulkan_debug->messenger_create_info(), dld );

         _vulkan_debug->setup_messenger( _instance->unq_vk_instance(), dld );
      }

      vulkan::vulkan()
        : vk_physical_device { VK_NULL_HANDLE }
        , _instance { std::make_unique<instance>() }
        , _vulkan_debug { std::make_unique<vulkan_debug>() }
      {}

      vulkan::~vulkan() {}
   }    // namespace vulkan

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
