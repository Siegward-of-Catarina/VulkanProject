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
      vkm::Queuefamily indices { device, surface, dld };
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
     : _dld { dld }, _physicaldevices {}, _queuefamily {}, _swapchain_details {}, _logical_device {}
   {
      for ( auto& device : physical_devices ) { _physicaldevices.emplace_back( device, 0u ); }
   }
   Device::~Device() {}
   void
   Device::pickUpSuitablePhysicalDevice( const UniqueSurface& surface )
   {
      uint16_t       hi_score { 0 };
      const uint16_t features_support_point { 100 };
      for ( auto& device : _physicaldevices )
      {
         if ( ::isSupportVulkanFeatures( device.first, surface, _dld ) ) { device.second += features_support_point; }

         auto dev_properties = device.first.getProperties();
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
      // �œK�ȃf�o�C�X�����������炱����ێ����Ă���
      _queuefamily       = std::make_unique<Queuefamily>( _suitable_physicaldevice.value(), surface, _dld );
      _swapchain_details = std::make_unique<SwapChainDetails>( _suitable_physicaldevice.value(), surface );
   }

   void
   Device::createLogicalDevice( const std::vector<const char*>& validationlayers )
   {
      std::vector<QueueCreateInfo> queue_createinfos;
      std::vector<uint32_t>        unq_queuefamilies { _queuefamily->graphicsfamily };
      // graphicsqueue �Ɓ@presentqueue �������ꍇ createDeviceUnique �ŗ�O����������Bqueuefamilyindex
      // �͈�ӂł���K�v������悤��.
      if ( _queuefamily->graphicsfamily != _queuefamily->presentfamily )
         unq_queuefamilies.emplace_back( _queuefamily->presentfamily );

      const float queue_priority { 1.f };

      for ( uint32_t queuefamily : unq_queuefamilies )
      {
         const QueueCreateInfo queue_createinfo { {}, queuefamily, 1, &queue_priority };
         queue_createinfos.emplace_back( queue_createinfo );
      }

      const PhysicalDeviceFeatures device_features {};

      const auto& device_extensions { ::device_exts };

      DeviceCreateInfo createinfo { {}, queue_createinfos, {}, device_extensions, &device_features };

      if ( !validationlayers.empty() ) createinfo.setPEnabledLayerNames( validationlayers );

      _logical_device = _suitable_physicaldevice.value().createDeviceUnique( createinfo, nullptr, _dld );

      if ( !_logical_device ) { throw std::runtime_error( "failed to create logical device!" ); }

      utl::log( "create logical device & valid queue succeeded." );
   }

   const vkm::Queue
   Device::getQueue( const QueuefamilyTypes& type )
   {
      assert( type < QueuefamilyTypes::COUNT );
      return _logical_device->getQueue( _queuefamily->select( type ), 0, _dld );
   }

}    // namespace my_library::vkm
// swapchaindetails
namespace my_library::vkm
{
   const bool
   SwapChainDetails::adequate()
   {
      return !this->_formats.empty() && !this->_present_modes.empty();
   }
   SurfaceFormatKHR
   SwapChainDetails::chooseSwapSurfaceFormat( const vkm::Format& format, const vkm::ColorSpace& color_space )
   {
      for ( auto& available_format : _formats )
      {
         if ( available_format.format == format && available_format.colorSpace == color_space )
         {
            return available_format;
         }
      }
      return _formats[ 0 ];
   }
   PresentModeKHR
   SwapChainDetails::chooseSwapPresentMode( const std::optional<PresentModeKHR>& present_mode )
   {
      if ( present_mode.has_value() )
      {
         for ( auto& available_mode : _present_modes )
         {
            if ( available_mode == present_mode.value() ) { return available_mode; }
         }
      }
      return PresentModeKHR::eFifo;
   }
   VkExtent2D
   SwapChainDetails::chooseSwapExtent( const std::pair<int, int>& glfw_fbuffer_size )
   {
      if ( _capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max() )
      {
         return _capabilities.currentExtent;
      }
      else
      {
         VkExtent2D actualExtent = { static_cast<uint32_t>( glfw_fbuffer_size.first ),
                                     static_cast<uint32_t>( glfw_fbuffer_size.second ) };
         actualExtent.width =
           std::clamp( actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width );
         actualExtent.height =
           std::clamp( actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height );

         return actualExtent;
      }
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
// queuefamily
namespace my_library::vkm
{
   const bool
   Queuefamily::is_complete()
   {
      return _graphicsfamily.has_value() && _presentfamily.has_value();
   }

   const uint32_t
   Queuefamily::select( const QueuefamilyTypes& type )
   {
      assert( type < QueuefamilyTypes::COUNT );
      switch ( type )
      {
         case QueuefamilyTypes::GRAPHICS_QUEUE : return _graphicsfamily.value(); break;
         case QueuefamilyTypes::PRESENT_QUEUE : return _presentfamily.value(); break;
         default : utl::runtimeError( "no types queuefamily found." ); break;
      }
      return 0;
   }
   Queuefamily::Queuefamily( const PhysicalDevice&        device,
                             const UniqueSurface&         surface,
                             const DispatchLoaderDynamic& dld )
     : graphicsfamily { _graphicsfamily.has_value() }, presentfamily { _presentfamily.has_value() }
   {
      const std::vector<vk::QueueFamilyProperties> queuefamilyes { device.getQueueFamilyProperties() };

      uint32_t i { 0 };
      for ( const auto& queuefamily : queuefamilyes )
      {
         // �f�o�C�X���O���t�B�b�N�֘A�̃R�}���h�ɑΉ����Ă��邩����
         if ( queuefamily.queueFlags & vk::QueueFlagBits::eGraphics ) _graphicsfamily = i;

         const VkBool32 present_support { device.getSurfaceSupportKHR( i, surface.get(), dld ) };
         if ( present_support ) { _presentfamily = i; }

         // �����莟��I��
         if ( is_complete() ) break;
         ++i;
      }
      if ( !is_complete() ) utl::runtimeError( "find queuefamily successed." );
   }
   Queuefamily::~Queuefamily() {}
}    // namespace my_library::vkm