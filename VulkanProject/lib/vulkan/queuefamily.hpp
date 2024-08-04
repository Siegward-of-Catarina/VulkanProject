#pragma once
#include "vkObjectTypes.hpp"
#include "utilities.hpp"

#include <optional>
namespace my_library::vkm::queuefamily
{
   enum class types
   {
      GRAPHICS_QUEUE,
      PRESENT_QUEUE,
      COUNT
   };

   struct indices
   {
      std::optional<uint32_t> graphicsfamily;
      std::optional<uint32_t> presentfamily;
      const bool
      is_complete();

      const std::optional<uint32_t>
      select( const types& type );
   };

   indices
   find_queuefamily( const PhysicalDevice& device, const UniqueSurface& surface, const DispatchLoaderDynamic& dld );

}    // namespace my_library::vulkan::queuefamily
