#include "queuefamily.hpp"

namespace my_library::vulkan::queuefamily
{
   bool
   indices::is_complete()
   {
      return graphicsfamily.has_value();
   }

   std::optional<uint32_t>
   indices::select( const types& type )
   {
      assert( type < types::COUNT );
      switch ( type )
      {
         case types::GRAPHICS_QUEUE : return this->graphicsfamily;
         case types::PRESENT_QUEUE : return 1;
         default : return my_library::utl::e_to_b( types::COUNT );
      }
   }

   indices
   find_queuefamily( const vk_physicaldevice& device )
   {
      queuefamily::indices                   indices;
      std::vector<vk::QueueFamilyProperties> queuefamilyes = device.getQueueFamilyProperties();

      int i { 0 };
      for ( const auto& queuefamily : queuefamilyes )
      {
         // �f�o�C�X���O���t�B�b�N�֘A�̃R�}���h�ɑΉ����Ă��邩����
         if ( queuefamily.queueFlags & vk::QueueFlagBits::eGraphics ) { indices.graphicsfamily = i; }
         // �����莟��I��
         if ( indices.is_complete() ) { break; }

         i++;
      }
      return indices;
   }
}    // namespace my_library::vulkan::queuefamily
