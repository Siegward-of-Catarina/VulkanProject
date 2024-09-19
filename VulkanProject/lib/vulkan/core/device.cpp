#include "device.hpp"

#include "../queuefamily.hpp"

#include <set>

namespace
{
   namespace vkm = my_library::vkm;

   const std::vector<const char*> device_exts { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

   const bool
   check_device_ext_support( const vkm::PhysicalDevice& device )
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
   isSupportVulkanFeatures( const vkm::PhysicalDevice&        device,
                            const vkm::UniqueSurface&         surface,
                            const vkm::DispatchLoaderDynamic& dld )
   {
      // キューファミリに対応しているかチェック
      vkm::queuefamily::indices indices { vkm::queuefamily::find_queuefamily( device, surface, dld ) };
      // デバイス固有の拡張機能に対応しているかチェック
      const bool ext_supported { ::check_device_ext_support( device ) };
      // swapchainの情報を取得できるかチェック

      if ( ext_supported )
      {
         vkm::SwapChainDetails details { device, surface };
         return indices.is_complete() && details.adequate();
      }
      return false;
   }

}    // namespace
namespace my_library::vkm
{
   Device::Device( std::vector<PhysicalDevice> physical_devices, const DispatchLoaderDynamic& dld )
     : _dld { dld }, _physicaldevices {}, _logical_device {}
   {
      for ( auto& device : physical_devices ) { _physicaldevices.emplace_back( device, 0u ); }
   }
   Device::~Device() {}
   void
   Device::pickUpSuitablePhysicalDevice( const UniqueSurface& surface )
   {
      uint16_t       hi_score { 0 };
      uint16_t       features_support_point { 100 };
      for ( auto& device : _physicaldevices )
      {
         if ( ::isSupportVulkanFeatures( device.first, surface, _dld ) ) { device.second += features_support_point; }

         auto dev_properties = device.first.getProperties();
         utl::log( dev_properties.deviceName );
         switch ( dev_properties.deviceType )
         {
            case vk::PhysicalDeviceType::eDiscreteGpu : device.second += 50; break;    // 外付けgpu 圧倒的に優先する
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
      if ( !_suitable_physicaldevice.has_value() ) { utl::runtimeError( "no availabble devices found." ); }
   }

}    // namespace my_library::vkm

namespace my_library::vkm
{
   const bool
   SwapChainDetails::adequate()
   {
      return !this->_formats.empty() && !this->_present_modes.empty();
   }

   SwapChainDetails::SwapChainDetails( const PhysicalDevice& device, const UniqueSurface& surface )
     : _capabilities { device.getSurfaceCapabilitiesKHR( surface.get() ) }
     , _formats { device.getSurfaceFormatsKHR( surface.get() ) }
     , _present_modes { device.getSurfacePresentModesKHR( surface.get() ) }
     , capabilities { _capabilities }
     , formats { _formats }
     , present_modes { _present_modes }
   {}

   SwapChainDetails::~SwapChainDetails() {}

}    // namespace my_library::vkm
