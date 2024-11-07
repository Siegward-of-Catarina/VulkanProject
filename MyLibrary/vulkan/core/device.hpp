#pragma once
#include "../vkObjectTypes.hpp"

#include <glm/vec2.hpp>

namespace my_library::vkm
{
   class Device
   {
   public:
      void
      pickUpSuitablePhysicalDevice( const surface::UniqueHandle& surface );
      void
      createLogicalDevice( const std::vector<const char*>& validationlayers );
      const device::GpuProperties
      getGPUProperties();
      const device::MemoryProperties
      getGpuMemoryProperties();
      Device( std::vector<device::Gpu> physical_devices, const DispatchLoaderDynamic& dld );
      ~Device();

   public:
      const device::UniqueHandle&   logical_device;
      const device::Gpu& suitable_physical_device;

   private:
      const DispatchLoaderDynamic&                      _dld;
      std::vector<std::pair<device::Gpu, uint16_t> > _physicaldevices;
      device::Gpu                                    _suitable_physicaldevice;
      device::UniqueHandle                                      _logical_device;
   };
}    // namespace my_library::vkm
