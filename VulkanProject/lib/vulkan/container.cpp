#include "container.hpp"
namespace my_library
{
   namespace vulkan
   {
      void
      container::register_ctr( unq_vk_instance& ins )
      {
         _instance = std::move( ins );
         _dld.init( *_instance );
      }
      void
      container::register_ctr( unq_vk_instance&& ins )
      {
         _instance = std::move( ins );
         _dld.init( *_instance );
      }
      void
      container::register_ctr( const vk_physicaldevice& dev )
      {
         _gpu = dev;
      }
      container::container() : dld { _dld }, instance { _instance }, gpu { _gpu } { _dld.init(); }
      container::~container() {}
   }    // namespace vulkan
}    // namespace my_library
