#include "logicaldevice.hpp"

#include "../container.hpp"
#include "../extension.hpp"
#include "../queuefamily.hpp"
#include "../utilities.hpp"
#include "../validationlayer.hpp"
#include "physicaldevice.hpp"

#include <set>
namespace my_library::vulkan
{
   void
   logicaldevice::init()
   {
      if ( const shared_container c = ctr_lock() )
      {
         const uint32_t& graphicsqueue { c->queuefamilies.graphicsfamily.value() };
         const uint32_t& presentqueue { c->queuefamilies.graphicsfamily.value() };

         std::vector<vk_queue_createinfo> queue_createinfos;
         std::vector<uint32_t>            unq_queuefamilies { graphicsqueue };
         // graphicsqueue と　presentqueue が同じ場合 createDeviceUnique で例外が発生する。queuefamilyindex
         // は一意である必要があるようだ.
         if ( graphicsqueue != presentqueue ) unq_queuefamilies.emplace_back( presentqueue );

         const float queue_priority { 1.f };

         for ( uint32_t queuefamily : unq_queuefamilies )
         {
            const vk_queue_createinfo queue_createinfo { {}, queuefamily, 1, &queue_priority };
            queue_createinfos.emplace_back( queue_createinfo );
         }

         const vk_physicaldevice_features device_features {};

         const auto& device_extensions { ext::get_device_exts() };

         vk_device_createinfo createinfo { {}, queue_createinfos, {}, device_extensions, &device_features };

         const auto validationlayers { layer::get_validationlayers() };
         if ( !validationlayers.empty() ) createinfo.setPEnabledLayerNames( validationlayers );

         _device = c->gpu.createDeviceUnique( createinfo, nullptr, c->dld );

         if ( !_device ) { throw std::runtime_error( "failed to create logical device!" ); }

         utl::log( "create logical device & valid queue succeeded." );
      }
   }

   const vk_queue
   logicaldevice::get_queue( const queuefamily ::types& type )
   {
      using types = queuefamily::types;

      if ( const shared_container c = ctr_lock() )
      {
         uint32_t idx;
         assert( type < types::COUNT );
         switch ( type )
         {
            case types::GRAPHICS_QUEUE : idx = c->queuefamilies.graphicsfamily.value(); break;
            case types::PRESENT_QUEUE : idx = c->queuefamilies.presentfamily.value(); break;
         }
         return _device->getQueue( idx, 0, c->dld );
      }

      return VK_NULL_HANDLE;
   }

   logicaldevice::logicaldevice( const shared_container& c ) : core_object { c } {}
   logicaldevice::~logicaldevice() {}
}    // namespace my_library::vulkan