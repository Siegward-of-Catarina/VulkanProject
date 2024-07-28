#include "queuefamily.hpp"
#include "utilities.hpp"
namespace my_library::vulkan::queuefamily
{
   const bool
   indices::is_complete()
   {
      return graphicsfamily.has_value() && presentfamily.has_value();
   }

   const std::optional<uint32_t>
   indices::select( const types& type )
   {
      assert( type < types::COUNT );
      switch ( type )
      {
         case types::GRAPHICS_QUEUE :
             return this->graphicsfamily;
         case types::PRESENT_QUEUE :
             return this->presentfamily; 
      }
      return std::optional<uint32_t>();
   }

   indices
   find_queuefamily( const vk_physicaldevice&         device,
                     const unq_vk_surface&            surface,
                     const vk_dispatchloader_dynamic& dld )
   {
      queuefamily::indices                   indices;
      const std::vector<vk::QueueFamilyProperties> queuefamilyes { device.getQueueFamilyProperties() };

      uint32_t i { 0 };
      for ( const auto& queuefamily : queuefamilyes )
      {
         // デバイスがグラフィック関連のコマンドに対応しているか検索
         if ( queuefamily.queueFlags & vk::QueueFlagBits::eGraphics ) indices.graphicsfamily = i;

         const VkBool32 present_support { device.getSurfaceSupportKHR( i, surface.get(), dld ) };
         if ( present_support )
         { 
             indices.presentfamily = i; 
         }

         // 見つかり次第終了
         if ( indices.is_complete() ) break;

         ++i;
      }
      return indices;
   }
}    // namespace my_library::vulkan::queuefamily
