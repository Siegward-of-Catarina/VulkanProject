#include "instance.hpp"

#include "../../../pch.hpp"
#include "../container.hpp"
#include "../debugUtils.hpp"
#include "../extension.hpp"
#include "../utilities.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
namespace my_library
{
   namespace vkm
   {
      class Instance::Impl
      {
      public:
         void
         createDebug( const vkm::instance::CreateInfo& create_info );
         void
         create( const vkm::instance::CreateInfo& create_info );
         explicit Impl( const shared_ptr_container& c );
         ~Impl();

      public:
         const shared_ptr_container&               _container;
         debug_utl::UniqueDebugUtilsMessengerEXT _debug_messenger;
      };

      void
      Instance::Impl::createDebug( const vkm::instance::CreateInfo& create_info )
      {
         const ApplicationInfo app_info( create_info.app_name.c_str(),
                                         VK_MAKE_VERSION( 1, 0, 0 ),
                                         create_info.app_name.c_str(),
                                         VK_MAKE_VERSION( 1, 0, 0 ),
                                         VK_API_VERSION_1_2 );

         assert( ext::checkExtensionSupport( create_info.extensions, _container->dld ) );
         assert( !create_info.validation_layers.empty() );

         const StructureChain<InstanceCreateInfo, debug_utl::MessengerCreateInfoEXT> createInfo {
            {{}, &app_info, create_info.validation_layers, create_info.extensions},
            create_info.debug_info
         };

         UniqueInstance instance { vk::createInstanceUnique(
           createInfo.get<InstanceCreateInfo>(), nullptr, _container->dld ) };
         _container->registerCtr( instance );
         //•K‚¸ƒRƒ“ƒeƒi“o˜^Œã¶¬
         _debug_messenger =
           _container->instance->createDebugUtilsMessengerEXTUnique( create_info.debug_info, nullptr, _container->dld );
      }
      void
      Instance::Impl::create( const vkm::instance::CreateInfo& create_info )
      {
         if ( true )
         {
            const ApplicationInfo app_info( create_info.app_name.c_str(),
                                            VK_MAKE_VERSION( 1, 0, 0 ),
                                            create_info.app_name.c_str(),
                                            VK_MAKE_VERSION( 1, 0, 0 ),
                                            VK_API_VERSION_1_2 );

            assert( ext::checkExtensionSupport( create_info.extensions, _container->dld ) );

            const vkm::InstanceCreateInfo createInfo { {}, &app_info, {}, create_info.extensions };
            UniqueInstance                instance { vk::createInstanceUnique( createInfo, nullptr, _container->dld ) };
            _container->registerCtr( instance );
         }
         else { utl::runtimeError( "failed create instance." ); }
      }

      Instance::Impl::Impl( const shared_ptr_container& c ) : _container { c } {}
      Instance::Impl::~Impl() {}
   }    // namespace vkm
}    // namespace my_library
namespace my_library
{
   namespace vkm
   {
      void
      Instance::init( const instance::CreateInfo& create_info, const bool debug )
      {
         if ( debug ) { _pimpl->createDebug( create_info ); }
         else { _pimpl->create( create_info ); }
      }
      Instance::Instance( const shared_ptr_container& c ) : _pimpl { std::make_unique<Impl>( c ) } {}
      Instance::~Instance() {}
   }    // namespace vkm

}    // namespace my_library
