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
      // これはvk_instanceが内部で破棄してくれるのでunique_ptrは使わない
      vk::PhysicalDevice _physical_device;
   };
}    // namespace my_library::vulkan
