#pragma once
#include"../vkObjectTypes.hpp"
#include <optional>
namespace my_library::vkm
{
   struct CurrentSyncObjects;

   class GraphicsQueue
   {
   public:
      explicit GraphicsQueue( const device::UniqueHandle& device, const DispatchLoaderDynamic& dld );
      void
      submit( const std::optional<CurrentSyncObjects>& syncs,
              const pipeline::StageFlagBits&           wait_stage,
              const command::UniqueBuffer&             buffer );
      void
      waitIdle();
      ~GraphicsQueue();
   private:
      const DispatchLoaderDynamic& _dld;
      const queue::vkQueue _queue;
   };

   class PresentQueue
   {
   public:
      explicit PresentQueue( const device::UniqueHandle& device, const DispatchLoaderDynamic& dld );
      bool
      present( const sync::UniqueSemaphore& render_finished_semaphore,
               const swapchain::UniqueHandle& swapchain,
               const uint32_t&        image_index );
      ~PresentQueue();
   private:
      const DispatchLoaderDynamic& _dld;
      const queue::vkQueue _queue;
   };
}    // namespace my_library::vkm
