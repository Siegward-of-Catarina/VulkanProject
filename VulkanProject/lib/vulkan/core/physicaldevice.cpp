#include "physicaldevice.hpp"

#include "../container.hpp"
#include "../extension.hpp"
#include "../queuefamily.hpp"
#include "../swap_chain_support.hpp"
#include "../utilities.hpp"

namespace my_library::vulkan
{
   bool
   physicaldevice::is_device_suitable( const vk_physicaldevice& device )
   {
      if ( const shared_container c = ctr_lock() )
      {
         // �T�|�[�g����Ă���f�o�C�X���Ή����Ă���L���[�t�@�~�����擾
         queuefamily::indices idxs = queuefamily::find_queuefamily( device, c->surface, c->dld );
         // �T�|�[�g����Ă���f�o�C�X�ŗL�̊g���@�\���擾
         const bool ext_supported = ext::check_device_ext_support( device );
         // �T�|�[�g����Ă���X���b�v�`�F�C���̏ڍׂ��擾
         swapchain_support::details swpc_support;
         if ( ext_supported ) { swpc_support = swapchain_support::query_details( device, c->surface ); }

         if ( idxs.is_complete() && ext_supported && swpc_support.adequate() )
         {
            c->register_ctr( idxs );
            return true;
         }
      }
      return false;
   }

   void
   physicaldevice::pick_physical_device()
   {
      if ( const shared_container c = ctr_lock() )
      {
         assert( c->instance );
         assert( c->surface );

         const std::vector<vk_physicaldevice> devices { c->instance->enumeratePhysicalDevices( c->dld ) };
         vk_physicaldevice                    suitable_dev;
         // GPU�̃��X�g����K�؂ȃf�o�C�X���擾
         for ( const auto& device : devices )
         {
            if ( is_device_suitable( device ) )
            {
               suitable_dev = device;
               break;
            }
         }

         if ( !suitable_dev ) { throw std::runtime_error( "failed to find a suitable GPU!" ); }
         // �R���e�i�ɓo�^
         c->register_ctr( suitable_dev );
         utl::log( "pickup valid physical devices succeeded." );
      }
   }

   physicaldevice::physicaldevice( const shared_container& c ) : core_object { c } {}
   physicaldevice::~physicaldevice() {}
}    // namespace my_library::vulkan
