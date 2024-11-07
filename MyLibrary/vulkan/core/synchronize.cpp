#include "synchronize.hpp"

#include "../../pch.hpp"
#include "../../utilities.hpp"
namespace my_library::vkm
{

   CurrentSyncObjects::CurrentSyncObjects( const sync::UniqueSemaphore& _image_available_semaphores,
                                           const sync::UniqueSemaphore& _render_finished_semaphores,
                                           const sync::UniqueFence&     _in_flight_fences )
     : image_available_semaphore { _image_available_semaphores }
     , render_finished_semaphore { _render_finished_semaphores }
     , in_flight_fence { _in_flight_fences }
   {}

   CurrentSyncObjects::CurrentSyncObjects()
     : image_available_semaphore {}, render_finished_semaphore {}, in_flight_fence {}
   {}
   CurrentSyncObjects::~CurrentSyncObjects() {}
}    // namespace my_library::vkm
namespace my_library::vkm
{
   void
   Synchronize::create( const int& max_frames_in_flight )
   {
      sync::SemaphoreCreateInfo semaphore_info {};
      sync::FenceCreateInfo     fence_info( sync::FenceCreateFlagBits::eSignaled );

      for ( int i { 0 }; i < max_frames_in_flight; ++i )
      {
         _image_available_semaphores.emplace_back( _device->createSemaphoreUnique( semaphore_info, nullptr, _dld ) );
         _render_finished_semaphores.emplace_back( _device->createSemaphoreUnique( semaphore_info, nullptr, _dld ) );
         _in_flight_fences.emplace_back( _device->createFenceUnique( fence_info, nullptr, _dld ) );
      }
      utl::runtimeError( !_image_available_semaphores.empty(), "faild to create _image_available_semaphore!" );
      utl::runtimeError( !_render_finished_semaphores.empty(), "faild to create semaphore!" );
      utl::runtimeError( !_in_flight_fences.empty(), "faild to create fence!" );

      utl::log( "crete sync objects succeeded." );
   }
   void
   Synchronize::waitForFences( const int& current_frame_idx )
   {
      auto result { _device->waitForFences(
        _in_flight_fences[ current_frame_idx ].get(), vk::True, UINT64_MAX, _dld ) };
      utl::runtimeError( ( result == vk::Result::eSuccess ), "faild to wait for fences!" );
   }
   void
   Synchronize::resetFences( const int& current_frame_idx )
   {
      _device->resetFences( _in_flight_fences[ current_frame_idx ].get(), _dld );
   }
   CurrentSyncObjects
   Synchronize::getCurrentObjects( const uint32_t& current_idx )
   {
      return CurrentSyncObjects( _image_available_semaphores[ current_idx ],
                                 _render_finished_semaphores[ current_idx ],
                                 _in_flight_fences[ current_idx ] );
   }
   Synchronize::Synchronize( const device::UniqueHandle& device, const DispatchLoaderDynamic& dld )
     : _device { device }
     , _dld { dld }
     , _image_available_semaphores {}
     , _render_finished_semaphores {}
     , _in_flight_fences {}
   {}
   Synchronize::~Synchronize() {}

}    // namespace my_library::vkm