#include "queuefamily.hpp"

namespace my_library::vulkan
{
   bool
   queuefamily_indices::is_complete()
   {
      return graphcsfamily.has_value();
   }

   queuefamily_indices
   queuefamily_indices::find_queuefamily( const vk_physicaldevice& device )
   {
      queuefamily_indices                    indices;
      std::vector<vk::QueueFamilyProperties> queuefamilyes = device.getQueueFamilyProperties();

      int i { 0 };
      for ( const auto& queuefamily : queuefamilyes )
      {
         // デバイスがグラフィック関連のコマンドに対応しているか検索
         if ( queuefamily.queueFlags & vk::QueueFlagBits::eGraphics ) { indices.graphcsfamily = i; }
         // 見つかり次第終了
         if ( indices.is_complete() ) { break; }

         i++;
      }
      return indices;
   }
}    // namespace my_library::vulkan
