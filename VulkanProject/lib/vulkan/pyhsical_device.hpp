#pragma once
#include "common.hpp"
namespace my_library::vulkan
{
   class pyhsical_device
   {
   private:
      bool
      is_device_suitable( const vk::PhysicalDevice& device );

   public:
      void
      pick_physical_device( UniqueInstance& instance, vk::DispatchLoaderDynamic& dld );

      pyhsical_device();
      ~pyhsical_device();

   private:
      // ‚±‚ê‚Ívk_instance‚ª“à•”‚Å”jŠü‚µ‚Ä‚­‚ê‚é‚Ì‚Åunique_ptr‚ÍŽg‚í‚È‚¢
      vk::PhysicalDevice _physical_device;
   };
}    // namespace my_library::vulkan
