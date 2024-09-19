#pragma once
#include "core_commonheaders.hpp"

#include <optional>
namespace my_library::vkm
{
   class SwapChainDetails
   {
   public:
      const bool
      adequate();
      explicit SwapChainDetails( const PhysicalDevice& device, const UniqueSurface& surface );
      ~SwapChainDetails();

   public:
      const SurfaceCapabilitiesKHR&        capabilities;
      const std::vector<SurfaceFormatKHR>& formats;
      const std::vector<PresentModeKHR>&   present_modes;

   private:
      SurfaceCapabilitiesKHR        _capabilities;
      std::vector<SurfaceFormatKHR> _formats;
      std::vector<PresentModeKHR>   _present_modes;
   };

   class Device
   {
   public:
      void
      pickUpSuitablePhysicalDevice( const UniqueSurface& surface );
      Device( std::vector<PhysicalDevice> physical_devices, const DispatchLoaderDynamic& dld );
      ~Device();

   private:
      const DispatchLoaderDynamic&                      _dld;
      std::vector<std::pair<PhysicalDevice, uint16_t> > _physicaldevices;
      std::optional<PhysicalDevice>                     _suitable_physicaldevice;
      UniqueDevice                                      _logical_device;
   };
}    // namespace my_library::vkm
