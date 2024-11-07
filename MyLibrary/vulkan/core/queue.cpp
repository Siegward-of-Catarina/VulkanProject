#include "queue.hpp"

#include "../../pch.hpp"
#include "../../utilities.hpp"
#include "queuefamily.hpp"
#include "synchronize.hpp"

namespace my_library::vkm
{
   GraphicsQueue::GraphicsQueue( const device::UniqueHandle& device, const DispatchLoaderDynamic& dld )
     : _queue { device->getQueue( Queuefamily::graphicsfamily(), 0, dld ) }, _dld { dld }
   {}
   void
   GraphicsQueue::submit( const std::optional<CurrentSyncObjects>& syncs,
                          const pipeline::StageFlagBits&           wait_stage,
                          const command::UniqueBuffer&             buffer )
   {
      if ( syncs.has_value() )
      {
         const auto&          sync = syncs.value();
         pipeline::StageFlags wait_stage_flags { wait_stage };
         queue::SubmitInfo    submit_info(
           sync.image_available_semaphore.get(), wait_stage_flags, buffer.get(), sync.render_finished_semaphore.get() );
         _queue.submit( submit_info, sync.in_flight_fence.get(), _dld );
      }
      else
      {
         queue::SubmitInfo submit_info {};
         submit_info.setCommandBuffers( buffer.get() );
         _queue.submit( submit_info, nullptr, _dld );
      }
   }
   void
   GraphicsQueue::waitIdle()
   {
      _queue.waitIdle( _dld );
   }
   GraphicsQueue::~GraphicsQueue() {}
   PresentQueue::PresentQueue( const device::UniqueHandle& device, const DispatchLoaderDynamic& dld )
     : _queue { device->getQueue( Queuefamily::presentfamily(), 0, dld ) }, _dld { dld }
   {}
   bool
   PresentQueue::present( const sync::UniqueSemaphore&   render_finished_semaphore,
                          const swapchain::UniqueHandle& swapchain,
                          const uint32_t&                image_index )
   {
      queue::PresentInfo info( render_finished_semaphore.get(), swapchain.get(), image_index );
      vk::Result         res {};
      try
      {
         res = _queue.presentKHR( info, _dld );
      } catch ( vk::OutOfDateKHRError eerr )
      {
         return true;
      } catch ( vk::SystemError err )
      {
         utl::runtimeError( "failed to present swap chain image!" );
      }
      return res == vk::Result::eSuboptimalKHR;
   }
   PresentQueue::~PresentQueue() {}
}    // namespace my_library::vkm