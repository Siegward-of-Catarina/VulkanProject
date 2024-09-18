#include "instance.hpp"

#include "../../../pch.hpp"
#include "../../utilities.hpp"
#include "../debugUtils.hpp"
#include "../extension.hpp"
#include "../queuefamily.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
namespace
{
   namespace vkm               = my_library::vkm;
   using UniqueInstance        = vkm::UniqueInstance;
   using InstanceCreateDatas   = vkm::instance::CreateInfo;
   using DispatchLoaderDynamic = vkm::DispatchLoaderDynamic;

   bool layerfound = false;

   const bool
   checkExtensionSupport( std::vector<const char*> extensions, const DispatchLoaderDynamic& dld )
   {
      // �g���@�\�̏����擾
      std::vector<vkm::ExtensionProperties> available_extensions { vk::enumerateInstanceExtensionProperties( nullptr,
                                                                                                             dld ) };
      // �g���@�\���T�|�[�g����Ă��邩�ꗗ����`�F�b�N
      for ( const char* extension : extensions )
      {
         bool extension_found { false };
         for ( const auto& extension_property : available_extensions )
         {
            if ( strcmp( extension, extension_property.extensionName ) == 0 )
            {
               extension_found = true;
               my_library::utl::log( "instance extensions is supported." );
               break;
            }
         }
         if ( !extension_found ) return false;
      }
      return true;
   }
   bool
   checkValidationLayerSupport( const std::vector<const char*>& validationlayers )
   {
      if ( validationlayers.empty() ) return false;

      const std::vector<vkm::LayerProperties> availablelayers { vk::enumerateInstanceLayerProperties() };
      bool                                    layerfound { false };
      for ( const char* layername : validationlayers )
      {
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
   const std::vector<const char*>
   getValidationLayers()
   {
#ifdef NDEBUG
      const std::vector<const char*> validationlayers {};
#else
      const std::vector<const char*> validationlayers { "VK_LAYER_KHRONOS_validation" };
      // ��x�����`�F�b�N����
      static const bool valid { checkValidationLayerSupport( validationlayers ) };

      if ( !valid ) throw std::runtime_error( "validation layers requested, but not available!" );
#endif
      return validationlayers;
   }

   UniqueInstance
   create( const InstanceCreateDatas& create_datas, const DispatchLoaderDynamic& dld )
   {
      const vkm::ApplicationInfo app_info( create_datas.app_name.c_str(),
                                           VK_MAKE_VERSION( 1, 0, 0 ),
                                           create_datas.app_name.c_str(),
                                           VK_MAKE_VERSION( 1, 0, 0 ),
                                           VK_API_VERSION_1_2 );

      assert( checkExtensionSupport( create_datas.extensions, dld ) );

      const vkm::InstanceCreateInfo createInfo { {}, &app_info, {}, create_datas.extensions };
      UniqueInstance                instance { vk::createInstanceUnique( createInfo, nullptr, dld ) };
      if ( !instance ) { my_library::utl::runtimeError( "failed create instance." ); }
      return instance;
   }
   UniqueInstance
   createDebug( const InstanceCreateDatas& create_datas, const DispatchLoaderDynamic& dld )
   {
      const vk::ApplicationInfo app_info( create_datas.app_name.c_str(),
                                          VK_MAKE_VERSION( 1, 0, 0 ),
                                          create_datas.app_name.c_str(),
                                          VK_MAKE_VERSION( 1, 0, 0 ),
                                          VK_API_VERSION_1_2 );

      assert( checkExtensionSupport( create_datas.extensions, dld ) );
      auto validation_layers = getValidationLayers();
      assert( !validation_layers.empty() );

      const vkm::StructureChain<vkm::InstanceCreateInfo, vkm::DebugUtilsMessengerCreateInfoEXT> create_instance_info {
         {{}, &app_info, validation_layers, create_datas.extensions},
         create_datas.debug_info
      };

      UniqueInstance instance =
        vk::createInstanceUnique( create_instance_info.get<vkm::InstanceCreateInfo>(), nullptr, dld );
      if ( !instance ) { my_library::utl::runtimeError( "failed create instance." ); }
      my_library::utl::log( "create vkInstance succeeded." );

      return instance;
   }
}    // namespace
namespace my_library
{
   namespace vkm
   {
      void
      Instance::create( const instance::CreateInfo& create_info, const bool debug )
      {
         if ( debug ) { _instance = std::move( ::createDebug( create_info, _dld ) ); }
         else { _instance = std::move( ::create( create_info, _dld ) ); }

         // �֐��|�C���^�擾
         _dld.init( *_instance );

         for ( auto& device : _instance->enumeratePhysicalDevices( _dld ) ) { _devices.emplace_back( device, 0u ); }
      }

      const PhysicalDevice
      Instance::pickUpSuitableDevice( const UniqueSurface& surface )
      {
         PhysicalDevice out {};
         uint16_t       hi_score { 0 };
         bool           queuefamilyvalid { false };
         for ( auto& device : _devices )
         {
            //
            queuefamily::indices indices { queuefamily::find_queuefamily( device.first, surface, _dld ) };
            if ( indices.is_complete() )
            {
               // ��x�ł��ʂ�΃T�|�[�g���Ă���f�o�C�X�͑��݂���B
               queuefamilyvalid = true;
               device.second += 10;
            }

            auto dev_properties = device.first.getProperties();
            switch ( dev_properties.deviceType )
            {
               case vk::PhysicalDeviceType::eDiscreteGpu : device.second = 100; break;    // �O�t��gpu ���|�I�ɗD�悷��
               case vk::PhysicalDeviceType::eIntegratedGpu : device.second = 50; break;    // cpu����gpu
               case vk::PhysicalDeviceType::eCpu : device.second = 10; break;              // cpu���g�p
               case vk::PhysicalDeviceType::eVirtualGpu : device.second = 5; break;        // ���zgpu
               case vk::PhysicalDeviceType::eOther : device.second = 0; break;    // ��̂ǂ�ɂ����Ă͂܂�Ȃ�
            }
         }

         if ( !queuefamilyvalid ) utl::runtimeError( "devices is not support queuefamilies" );
      }

      Instance::Instance() : dld { _dld }, vkobj { _instance }, _devices {} { _dld.init(); }
      Instance::~Instance() {}
   }    // namespace vkm

}    // namespace my_library
