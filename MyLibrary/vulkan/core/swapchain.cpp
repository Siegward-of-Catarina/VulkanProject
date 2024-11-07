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

      // スワップチェーンに含める画像の数を決定
      // NOTE: 画像数を最小値に固執するとドライバが内部操作を完了するのを待つ可能性が出てくるので、
      // 最小値より1つ多い数を指定しておくといい
      uint32_t image_count = swapchain_details.capabilities.minImageCount + 1;
      // サポートされている最大個数が0でないかつ、image_count が最大数を超えていた場合は、最大数に丸める。
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
        1, /*imageArrayLayers これはvrアプリケーションの開発以外は基本的に１のようだ*/
        swapchain::ImageUsageFlagBits::eColorAttachment, /*画像に直接レンダリングする*/
        queue::SharingMode::eExclusive,
        {},
        swapchain_details.capabilities
          .currentTransform, /*ディスプレイとFバッファの向きがことなる場合がある。変更が不要なら現在の向きを指定する。*/
        swapchain::CompositeAlphaFlagBits::eOpaque, /*今のところアルファブレントは無視*/
        present_mode,
        VK_TRUE,                                    /*clipped*/
        VK_NULL_HANDLE /*今のところnullptr*/ );

      // グラフィックスとプレゼントファミリのindexがことなる場合は
      // イメージを複数のキューで共有するようにする
      if ( Queuefamily::graphicsfamily() != Queuefamily::presentfamily() )
      {
         createinfo.setImageSharingMode( queue::SharingMode::eConcurrent );
         std::vector indices { Queuefamily::graphicsfamily(), Queuefamily::presentfamily() };
         createinfo.setQueueFamilyIndices( indices );
      }
      _swapchain = _device->createSwapchainKHRUnique( createinfo, nullptr, _dld );
      if ( !_swapchain ) utl::runtimeError( "failed to create swap chain!" );

      // ハンドル取得
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
      // 画像の目的とアクセスする場所を指定する。今回はカラーターゲットでミップマップなし、単一レイヤで使用
      image_view::ImageSubresourceRange subresourceRange( image_view::ImageAspectFlagBits::eColor, 0, 1, 0, 1 );

      // リストサイズをフィット
      _imageviews.reserve( _images.size() );

      for ( int i { 0 }; i < _images.size(); ++i )
      {
         image_view::CreateInfo create_info( {},
                                             _images[ i ],
                                             image_view::Type::e2D,    // 画像データの解釈方法
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
      //現在のimage index を更新
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