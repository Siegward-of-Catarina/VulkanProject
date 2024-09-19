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
      // �v�����ꂽ�g���@�\�z�񂩂�L���Ȋg���@�\��������΍폜���Ă����B
      for ( const auto& ext : available_exts ) { required_exts.erase( ext.extensionName ); }

      my_library::utl::log( "requested device extensions is a suported." );

      // ���ׂėL���������ꍇ empty �ɂȂ�
      return required_exts.empty();
   }

   const bool
   isSupportVulkanFeatures( const vkm::PhysicalDevice&        device,
                            const vkm::UniqueSurface&         surface,
                            const vkm::DispatchLoaderDynamic& dld )
   {
      // �L���[�t�@�~���ɑΉ����Ă��邩�`�F�b�N
      vkm::queuefamily::indices indices { vkm::queuefamily::find_queuefamily( device, surface, dld ) };
      // �f�o�C�X�ŗL�̊g���@�\�ɑΉ����Ă��邩�`�F�b�N
      const bool ext_supported { ::check_device_ext_support( device ) };
      // swapchain�̏����擾�ł��邩�`�F�b�N

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
            case vk::PhysicalDeviceType::eDiscreteGpu : device.second += 50; break;    // �O�t��gpu ���|�I�ɗD�悷��
            case vk::PhysicalDeviceType::eIntegratedGpu : device.second += 20; break;    // cpu����gpu
            case vk::PhysicalDeviceType::eCpu : device.second += 10; break;              // cpu���g�p
            case vk::PhysicalDeviceType::eVirtualGpu : device.second += 5; break;        // ���zgpu
            case vk::PhysicalDeviceType::eOther : device.second += 1; break;    // ��̂ǂ�ɂ����Ă͂܂�Ȃ�
         }
         // �O��Ƃ���vulkan�̋@�\�ɑΉ����Ă���ׂ��B
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
