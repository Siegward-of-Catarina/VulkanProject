#include "device.hpp"

#include "../../pch.hpp"
#include "../../utilities.hpp"
#include "queuefamily.hpp"
#include "swapChainDetails.hpp"

#include <optional>
#include <set>

namespace
{
   namespace vkm = my_library::vkm;

   const std::vector<const char*> device_exts { VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_EXT_extended_dynamic_state3" };

   const bool
   check_device_ext_support( const vkm::device::Gpu& device )
   {
      const std::vector<my_library::vkm::ExtensionProperties> available_exts {
         device.enumerateDeviceExtensionProperties()
      };

      std::set<std::string> required_exts { device_exts.begin(), device_exts.end() };
      // 要求された拡張機能配列から有効な拡張機能が見つかれば削除していく。
      for ( const auto& ext : available_exts ) { required_exts.erase( ext.extensionName ); }

      my_library::utl::log( "requested device extensions is a suported." );

      // すべて有効だった場合 empty になる
      return required_exts.empty();
   }

   const bool
   isSupportVulkanFeatures( const vkm::device::Gpu&           device,
                            const vkm::surface::UniqueHandle& surface,
                            const vkm::DispatchLoaderDynamic& dld )
   {
      // キューファミリに対応しているかチェック
      vkm::Queuefamily indices { device, surface, dld };
      // デバイス固有の拡張機能に対応しているかチェック
      const bool ext_supported { ::check_device_ext_support( device ) };
      // swapchainの情報を取得できるかチェック
      vkm::SwapChainDetails details {};
      if ( ext_supported ) { details = vkm::SwapChainDetails { device, surface }; }
      return indices.is_complete() && ext_supported && details.adequate();
   }

}    // namespace
     // device
namespace my_library::vkm
{
   Device::Device( std::vector<device::Gpu> physical_devices, const DispatchLoaderDynamic& dld )
     : _dld { dld }
     , _physicaldevices {}
     , _logical_device {}
     , logical_device { _logical_device }
     , suitable_physical_device { _suitable_physicaldevice }
   {
      for ( auto& device : physical_devices ) { _physicaldevices.emplace_back( device, 0u ); }
   }
   Device::~Device() { utl::log( "delete device." ); }
   void
   Device::pickUpSuitablePhysicalDevice( const surface::UniqueHandle& surface )
   {
      uint16_t       hi_score { 0 };
      const uint16_t features_support_point { 100 };
      for ( auto& device : _physicaldevices )
      {
         if ( ::isSupportVulkanFeatures( device.first, surface, _dld ) ) { device.second += features_support_point; }

         auto dev_properties = device.first.getProperties();
         switch ( dev_properties.deviceType )
         {
            case vk::PhysicalDeviceType::eDiscreteGpu : device.second += 50; break;      // 外付けgpu 優先する
            case vk::PhysicalDeviceType::eIntegratedGpu : device.second += 20; break;    // cpu内部gpu
            case vk::PhysicalDeviceType::eCpu : device.second += 10; break;              // cpuを使用
            case vk::PhysicalDeviceType::eVirtualGpu : device.second += 5; break;        // 仮想gpu
            case vk::PhysicalDeviceType::eOther : device.second += 1; break;    // 上のどれにも当てはまらない
         }
         // 前提としてvulkanの機能に対応しているべき。
         if ( device.second > features_support_point )
         {
            if ( hi_score < device.second )
            {
               _suitable_physicaldevice = device.first;
               hi_score                 = device.second;
            }
         }
      }
      if ( !std::optional( _suitable_physicaldevice ).has_value() )
      {
         utl::runtimeError( "no availabble devices found." );
      }
   }

   void
   Device::createLogicalDevice( const std::vector<const char*>& validationlayers )
   {
      std::vector<queue::CreateInfo> queue_createinfos;
      std::vector<uint32_t>          unq_queuefamilies { Queuefamily::graphicsfamily() };
      // graphicsqueue と　presentqueue が同じ場合 createDeviceUnique で例外が発生する。queuefamilyindex
      // は一意である必要があるようだ.
      if ( Queuefamily::graphicsfamily() != Queuefamily::presentfamily() )
         unq_queuefamilies.emplace_back( Queuefamily::presentfamily() );

      const float queue_priority { 1.f };

      for ( uint32_t queuefamily : unq_queuefamilies )
      {
         const queue::CreateInfo queue_createinfo { {}, queuefamily, 1, &queue_priority };
         queue_createinfos.emplace_back( queue_createinfo );
      }

      device::GpuFeatures device_features {};
      device_features.fillModeNonSolid = vk::True;// line point有効化
      device::GpuExtendedDynamicState3Features dynamic_state_features {};
      dynamic_state_features.extendedDynamicState3PolygonMode = vk::True; // dynamicstate polymode 有効化

      const auto& device_extensions { ::device_exts };

      device::CreateInfo createinfo { {}, queue_createinfos, {}, device_extensions, &device_features, &dynamic_state_features };

      if ( !validationlayers.empty() ) createinfo.setPEnabledLayerNames( validationlayers );

      _logical_device = _suitable_physicaldevice.createDeviceUnique( createinfo, nullptr, _dld );

      if ( !_logical_device ) { throw std::runtime_error( "failed to create logical device!" ); }
      utl::log( "create logical device & valid queue succeeded." );
   }

   const device::GpuProperties
   Device::getGPUProperties()
   {
      return _suitable_physicaldevice.getProperties();
   }

   const device::MemoryProperties
   Device::getGpuMemoryProperties()
   {
      return _suitable_physicaldevice.getMemoryProperties( _dld );
   }

}    // namespace my_library::vkm
