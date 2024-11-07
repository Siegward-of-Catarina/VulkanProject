#include "swapchain.hpp"

#include "../../pch.hpp"
#include "../../utilities.hpp"
#include "queuefamily.hpp"
#include "swapChainDetails.hpp"

namespace my_library::vkm
{
   void
   SwapChain::create( const device::Gpu&           device,
                      const surface::UniqueHandle& surface,
                      const glm::u32vec2&          glfw_fbuffer_size )
   {
      auto swapchain_details = SwapChainDetails { device, surface };
      auto surf_format       = swapchain_details.chooseSwapSurfaceFormat();
      auto present_mode      = swapchain_details.chooseSwapPresentMode( swapchain::PresentMode::eMailbox );
      auto extent            = swapchain_details.chooseSwapExtent( glfw_fbuffer_size );

      // �X���b�v�`�F�[���Ɋ܂߂�摜�̐�������
      // NOTE: �摜�����ŏ��l�ɌŎ�����ƃh���C�o�������������������̂�҂\�����o�Ă���̂ŁA
      // �ŏ��l���1���������w�肵�Ă����Ƃ���
      uint32_t image_count = swapchain_details.capabilities.minImageCount + 1;
      // �T�|�[�g����Ă���ő����0�łȂ����Aimage_count ���ő吔�𒴂��Ă����ꍇ�́A�ő吔�Ɋۂ߂�B
      if ( swapchain_details.capabilities.maxImageCount > 0 )
      {
         image_count = std::clamp( image_count, image_count, swapchain_details.capabilities.maxImageCount );
      }

      swapchain::CreateInfo createinfo(
        {},
        surface.get(),
        image_count,
        surf_format.format,
        surf_format.colorSpace,
        extent,
        1, /*imageArrayLayers �����vr�A�v���P�[�V�����̊J���ȊO�͊�{�I�ɂP�̂悤��*/
        swapchain::ImageUsageFlagBits::eColorAttachment, /*�摜�ɒ��ڃ����_�����O����*/
        queue::SharingMode::eExclusive,
        {},
        swapchain_details.capabilities
          .currentTransform, /*�f�B�X�v���C��F�o�b�t�@�̌��������ƂȂ�ꍇ������B�ύX���s�v�Ȃ猻�݂̌������w�肷��B*/
        swapchain::CompositeAlphaFlagBits::eOpaque, /*���̂Ƃ���A���t�@�u�����g�͖���*/
        present_mode,
        VK_TRUE,                                    /*clipped*/
        VK_NULL_HANDLE /*���̂Ƃ���nullptr*/ );

      // �O���t�B�b�N�X�ƃv���[���g�t�@�~����index�����ƂȂ�ꍇ��
      // �C���[�W�𕡐��̃L���[�ŋ��L����悤�ɂ���
      if ( Queuefamily::graphicsfamily() != Queuefamily::presentfamily() )
      {
         createinfo.setImageSharingMode( queue::SharingMode::eConcurrent );
         std::vector indices { Queuefamily::graphicsfamily(), Queuefamily::presentfamily() };
         createinfo.setQueueFamilyIndices( indices );
      }
      _swapchain = _device->createSwapchainKHRUnique( createinfo, nullptr, _dld );
      if ( !_swapchain ) utl::runtimeError( "failed to create swap chain!" );

      // �n���h���擾
      _images = _device->getSwapchainImagesKHR( _swapchain.get() );
      _format = surf_format.format;
      _extent = extent;

      utl::log( "create swap chain succeeded." );
   }
   void
   SwapChain::cleanup()
   {
      for ( auto& buffer : _framebuffers ) _device->destroyFramebuffer( buffer.release() );
      _framebuffers.clear();
      for ( auto& view : _imageviews ) _device->destroyImageView( view.release() );
      _imageviews.clear();
      _device->destroySwapchainKHR( _swapchain.release() );
   }
   void
   SwapChain::createImageViews()
   {
      image_view::ComponentMapping component( image_view::ComponentSwizzle::eIdentity,
                                              image_view::ComponentSwizzle::eIdentity,
                                              image_view::ComponentSwizzle::eIdentity,
                                              image_view::ComponentSwizzle::eIdentity );
      // �摜�̖ړI�ƃA�N�Z�X����ꏊ���w�肷��B����̓J���[�^�[�Q�b�g�Ń~�b�v�}�b�v�Ȃ��A�P�ꃌ�C���Ŏg�p
      image_view::ImageSubresourceRange subresourceRange( image_view::ImageAspectFlagBits::eColor, 0, 1, 0, 1 );

      // ���X�g�T�C�Y���t�B�b�g
      _imageviews.reserve( _images.size() );

      for ( int i { 0 }; i < _images.size(); ++i )
      {
         image_view::CreateInfo create_info( {},
                                             _images[ i ],
                                             image_view::Type::e2D,    // �摜�f�[�^�̉��ߕ��@
                                             _format,
                                             component,
                                             subresourceRange );
         _imageviews.emplace_back( _device->createImageViewUnique( create_info, nullptr, _dld ) );
         utl::runtimeError( _imageviews[ i ]->operator bool(), "failed to create image views!" );
      }
      utl::log( "create image views succeeded." );
   }
   void
   SwapChain::createFramebuffer( const render_pass::UniqueHandle& render_pass )
   {
      _framebuffers.reserve( _imageviews.size() );
      for ( auto const& imageview : _imageviews )
      {
         framebuffer::CreateInfo create_info(
           {}, render_pass.get(), imageview.get(), _extent.width, _extent.height, 1 );
         auto buffer { _device->createFramebufferUnique( create_info, nullptr, _dld ) };
         utl::runtimeError( buffer->operator bool(), "failed to create framebuffer!" );
         _framebuffers.emplace_back( std::move( buffer ) );
      }
      utl::log( "create frambuffer succeeded." );
   }
   vk::Result
   SwapChain::updateImageIndex( const sync::UniqueSemaphore& image_available_semaphore )
   {
      vk::ResultValue<uint32_t> resval { {}, {} };
      try
      {
         resval =
           _device->acquireNextImageKHR( _swapchain.get(), UINT64_MAX, image_available_semaphore.get(), {}, _dld );
      } catch ( vk::OutOfDateKHRError err )
      {
         resval.result = vk::Result::eErrorOutOfDateKHR;
      } catch ( vk::SystemError err )
      {
         utl::runtimeError( "failed to acquire swap chain image!" );
      }
      //���݂�image index ���X�V
      _current_image_index = resval.value;
      return resval.result;
   }
   void
   SwapChain::updateFrameIndex( const int& max_frames_in_flight )
   {
      _current_frame_index = ( _current_frame_index + 1 ) % max_frames_in_flight;
   }

   SwapChain::SwapChain( const device::UniqueHandle& device, const DispatchLoaderDynamic& dld )
     : _device { device }
     , _dld { dld }
     , _current_frame_index { 0 }
     , _current_image_index {}
     , _swapchain {}
     , _format {}
     , _extent {}
     , _images {}
     , _imageviews {}
     , _framebuffers {}
     , current_frame_index { _current_frame_index }
     , current_image_index { _current_image_index }
     , vk_swapchain { _swapchain }
     , format { _format }
     , extent { _extent }
     , images { _images }
     , framebuffers { _framebuffers }
   {}
   SwapChain::~SwapChain() {}
}    // namespace my_library::vkm