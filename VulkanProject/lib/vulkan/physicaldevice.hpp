#pragma once
#include "common.hpp"
namespace my_library::vulkan
{
   class physicaldevice
   {
   private:
      bool
      is_device_suitable( const vk_physicaldevice& device );

   public:
      void
      pick_physical_device( unq_vk_instance& instance, vk::DispatchLoaderDynamic& dld );

      physicaldevice();
      ~physicaldevice();

   private:
      // ‚±‚ê‚Ívk_instance‚ª“à•”‚Å”jŠü‚µ‚Ä‚­‚ê‚é‚Ì‚Åunique_ptr‚ÍŽg‚í‚È‚¢
      vk_physicaldevice _physical_device;
   };
}    // namespace my_library::vulkan
