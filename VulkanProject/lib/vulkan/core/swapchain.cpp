#include "swapchain.hpp"

namespace my_library::vkm
{
   void
   SwapChain::create( const SwapChainInfo info )
   {
      auto swapchain_details = info.swapchain_details;
      auto surf_format       = swapchain_details.chooseSwapSurfaceFormat();
      auto present_mode      = swapchain_details.chooseSwapPresentMode( PresentModeKHR::eMailbox );
      auto extent            = swapchain_details.chooseSwapExtent( info.glfw_fbuffer_size );

      // �X���b�v�`�F�[���Ɋ܂߂�摜�̐�������
      // NOTE: �摜�����ŏ��l�ɌŎ�����ƃh���C�o�������������������̂�҂\�����o�Ă���̂ŁA
      // �ŏ��l���1���������w�肵�Ă����Ƃ���
      uint32_t image_count = swapchain_details.capabilities.minImageCount + 1;
      // �T�|�[�g����Ă���ő����0�łȂ����Aimage_count ���ő吔�𒴂��Ă����ꍇ�́A�ő吔�Ɋۂ߂�B
      if ( swapchain_details.capabilities.maxImageCount > 0 )
      {
         image_count = std::clamp( image_count, image_count, swapchain_details.capabilities.maxImageCount );
      }

      SwapChainCreateInfo createinfo(
        {},
        info.surface.get(),
        image_count,
        surf_format.format,
        surf_format.colorSpace,
        extent,
        1, /*imageArrayLayers �����vr�A�v���P�[�V�����̊J���ȊO�͊�{�I�ɂP�̂悤��*/
        ImageUsageFlagBits::eColorAttachment, /*�摜�ɒ��ڃ����_�����O����*/
        SharingMode::eExclusive,
        {},
        swapchain_details.capabilities
          .currentTransform, /*�f�B�X�v���C��F�o�b�t�@�̌��������ƂȂ�ꍇ������B�ύX���s�v�Ȃ猻�݂̌������w�肷��B*/
        CompositeAlphaFlagBits::eOpaque, /*���̂Ƃ���A���t�@�u�����g�͖���*/
        present_mode,
        VK_TRUE, /*clipped*/
        VK_NULL_HANDLE /*���̂Ƃ���nullptr*/ );

      const uint32_t graphicsfamily = info.queuefamily.graphicsfamily;
      const uint32_t presentfamily  = info.queuefamily.presentfamily;
      // �O���t�B�b�N�X�ƃv���[���g�t�@�~����index�����ƂȂ�ꍇ��
      // �C���[�W�𕡐��̃L���[�ŋ��L����悤�ɂ���
      if ( graphicsfamily != presentfamily )
      {
         createinfo.setImageSharingMode( SharingMode::eConcurrent );
         std::vector indices { graphicsfamily, presentfamily };
         createinfo.setQueueFamilyIndices( indices );
      }
   }
   SwapChain::SwapChain() {}
   SwapChain::~SwapChain() {}
}    // namespace my_library::vkm