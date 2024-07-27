#pragma once
#include "vk_object_types.hpp"
#include "utilities.hpp"

#include <optional>
namespace my_library::vulkan::queuefamily
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
   find_queuefamily( const vk_physicaldevice& device, const unq_vk_surface& surface, const vk_dispatchloader_dynamic& dld );

}    // namespace my_library::vulkan::queuefamily
