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
      // �����vk_instance�������Ŕj�����Ă����̂�unique_ptr�͎g��Ȃ�
      vk::PhysicalDevice _physical_device;
   };
}    // namespace my_library::vulkan
