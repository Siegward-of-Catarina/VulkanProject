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
      SurfaceFormatKHR
      chooseSwapSurfaceFormat( const vkm::Format&     format = vkm::Format::eB8G8R8A8Srgb,
                               const vkm::ColorSpace& color_space = vkm::ColorSpace::eSrgbNonlinear );
      PresentModeKHR
      chooseSwapPresentMode( const std::optional<PresentModeKHR>& present_mode );
      VkExtent2D
      chooseSwapExtent( const std::pair<int, int>& glfw_fbuffer_size );
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
   enum class QueuefamilyTypes
   {
      GRAPHICS_QUEUE,
      PRESENT_QUEUE,
      COUNT
   };
   class Queuefamily
   {
   public:
      const bool
      is_complete();

      const uint32_t
      select( const QueuefamilyTypes& type );

      explicit Queuefamily( const PhysicalDevice&        device,
                            const UniqueSurface&         surface,
                            const DispatchLoaderDynamic& dld );
      ~Queuefamily();

   public:
      const uint32_t& graphicsfamily;
      const uint32_t& presentfamily;

   private:
      std::optional<uint32_t> _graphicsfamily;
      std::optional<uint32_t> _presentfamily;
   };
   class Device
   {
   public:
      void
      pickUpSuitablePhysicalDevice( const UniqueSurface& surface );
      void
      createLogicalDevice( const std::vector<const char*>& validationlayers );
      const vkm::Queue
      getQueue( const QueuefamilyTypes& type );
      Device( std::vector<PhysicalDevice> physical_devices, const DispatchLoaderDynamic& dld );
      ~Device();

   private:
      const DispatchLoaderDynamic&                      _dld;
      std::vector<std::pair<PhysicalDevice, uint16_t> > _physicaldevices;
      std::optional<PhysicalDevice>                     _suitable_physicaldevice;
      std::unique_ptr<Queuefamily>                      _queuefamily;
      std::unique_ptr<SwapChainDetails>                 _swapchain_details;
      UniqueDevice _logical_device;
   };
}    // namespace my_library::vkm
