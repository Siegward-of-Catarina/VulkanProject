#pragma once
#include "../vkObjectTypes.hpp"
namespace my_library::vkm
{
   struct CurrentSyncObjects
   {
      const sync::UniqueSemaphore& image_available_semaphore;
      const sync::UniqueSemaphore& render_finished_semaphore;
      const sync::UniqueFence&     in_flight_fence;
      explicit CurrentSyncObjects( const sync::UniqueSemaphore& _image_available_semaphores,
                            const sync::UniqueSemaphore& _render_finished_semaphores,
                            const sync::UniqueFence&     _in_flight_fences );
      CurrentSyncObjects();
      ~CurrentSyncObjects();
   };
   class Synchronize
   {
   public:
      void
      create( const int& max_frames_in_flight );
      void
      waitForFences( const int& current_frame_idx );
      void
      resetFences( const int& current_frame_idx );
      CurrentSyncObjects
      getCurrentObjects(const uint32_t& current_idx);
      Synchronize( const device::UniqueHandle& device, const DispatchLoaderDynamic& dld );
      ~Synchronize();

   private:
      const device::UniqueHandle&          _device;
      const DispatchLoaderDynamic& _dld;

      std::vector<sync::UniqueSemaphore> _image_available_semaphores;
      std::vector<sync::UniqueSemaphore> _render_finished_semaphores;
      std::vector<sync::UniqueFence>     _in_flight_fences;
   };
}    // namespace my_library::vkm
