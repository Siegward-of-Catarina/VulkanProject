#include "swap_chain.hpp"

#include "../../../pch.hpp"
#include "../container.hpp"
#include "../swap_chain_support.hpp"

namespace my_library
{
   namespace vulkan
   {
      void
      swap_chain::init( const std::pair<int, int>& glfw_fbuffer_size )
      {
         if ( const shared_container c = ctr_lock() )
         {
            // �X���b�v�`�F�[�����쐬���邽�߂̏ڍ׏����擾����
            const swapchain_support::details& swp_detailes = swapchain_support::query_details(c->gpu, c->surface);

            vk_surface_format_khr             surf_format  = swp_detailes.choose_swap_surfaceformat();
            vk_present_mode_khr present_mode = swp_detailes.choose_swap_presentmode( vk_presentmode_khr::eMailbox );
            vk_extent_2d        extent       = swp_detailes.choose_swap_extent( glfw_fbuffer_size );

            // �X���b�v�`�F�[���Ɋ܂߂�摜�̐�������
            // NOTE: �摜�����ŏ��l�ɌŎ�����ƃh���C�o�������������������̂�҂\�����o�Ă���̂ŁA
            // �ŏ��l���1���������w�肵�Ă����Ƃ���
            uint32_t image_count = swp_detailes.capabilities.minImageCount + 1;
            // �T�|�[�g����Ă���ő����0�łȂ����Aimage_count ���ő吔�𒴂��Ă����ꍇ�́A�ő吔�Ɋۂ߂�B
            if ( swp_detailes.capabilities.maxImageCount > 0 )
            {
               image_count = std::clamp( image_count, image_count, swp_detailes.capabilities.maxImageCount );
            }

            vk_swp_createinfo_khr createinfo(
              {},
              c->surface.get(),
              image_count,
              surf_format.format,
              surf_format.colorSpace,
              extent,
              1, /*imageArrayLayers �����vr�A�v���P�[�V�����̊J���ȊO�͊�{�I�ɂP�̂悤��*/
              vk_img_usageflag_bits::eColorAttachment, /*�摜�ɒ��ڃ����_�����O����*/
              vk_sharing_mode::eExclusive,
              {},
              swp_detailes.capabilities
                .currentTransform, /*�f�B�X�v���C��F�o�b�t�@�̌��������ƂȂ�ꍇ������B�ύX���s�v�Ȃ猻�݂̌������w�肷��B*/
              vk_comp_alphaflag_bits_khr::eOpaque, /*���̂Ƃ���A���t�@�u�����g�͖���*/
              present_mode,
              VK_TRUE,                             /*clipped*/
              VK_NULL_HANDLE /*���̂Ƃ���nullptr*/ );

            uint32_t graphicsfamily = c->queuefamilies.graphicsfamily.value();
            uint32_t presentfamily  = c->queuefamilies.presentfamily.value();
            // �O���t�B�b�N�X�ƃv���[���g�t�@�~����index�����ƂȂ�ꍇ��
            // �C���[�W�𕡐��̃L���[�ŋ��L����悤�ɂ���
            if ( graphicsfamily != presentfamily )
            {
               createinfo.setImageSharingMode( vk_sharing_mode::eConcurrent );
               std::vector indices { graphicsfamily, presentfamily };
               createinfo.setQueueFamilyIndices( indices );
            }

         }
      }
      swap_chain::swap_chain( const shared_container& c ) : core_object { c } {}
      swap_chain::~swap_chain() {}
   }    // namespace vulkan
}    // namespace my_library
