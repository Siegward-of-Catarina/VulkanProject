#include "pipeline.hpp"

#include "../../pch.hpp"
#include "../../shader/basicShader.hpp"
#include "../../utilities.hpp"
namespace my_library::vkm
{
   void
   Pipeline::createRenderPass( const image::Format& format )
   {
      render_pass::AttachmentDescription color_attachment(
        {},
        format,                                    // format : スワップチェインと一致させる
        pipeline::SampleCountFlagBits::e1,         // samples : マルチサンプリング用
        render_pass::AttachmentLoadOp::eClear,     // loadOp : レンダリング前にデータの扱い方を決める
        render_pass::AttachmentStoreOp::eStore,    // storeOp : レンダリング後のでーたの扱い方を決める
        render_pass::AttachmentLoadOp::eDontCare,     // stencilLoadOp : ステンシルバッファに対する loadOp
        render_pass::AttachmentStoreOp::eDontCare,    // stencilStoreOp : ステンシルバッファに対する storeOp
        render_pass::ImageLayout::eUndefined,         // initiallayout : パス開始前のピクセルレイアウト
        render_pass::ImageLayout::ePresentSrcKHR      // finalLayout ; パス終了後のピクセルレイアウト
      );

      // ==== sub pass =============
      render_pass::AttachmentReference color_attachment_ref(
        0,    // attachment : attachmentDescription配列のインデックス
        render_pass::ImageLayout::eColorAttachmentOptimal    // layout : attachmentの使用方法
      );
      std::vector<render_pass::AttachmentReference> refs { color_attachment_ref };
      render_pass::SubpassDescription               subpass( {}, pipeline::BindPoint::eGraphics, {}, refs );

      render_pass::SubpassDependency subpass_dependency( vk::SubpassExternal,
                                                         0,
                                                         pipeline::StageFlagBits::eColorAttachmentOutput,
                                                         pipeline::StageFlagBits::eColorAttachmentOutput,
                                                         {},
                                                         render_pass::AccessFlagBits::eColorAttachmentWrite );
      // ==== render pass ======================
      render_pass::CreateInfo renderpass_info( {}, color_attachment, subpass, subpass_dependency );
      _renderpass = _device->createRenderPassUnique( renderpass_info, nullptr, _dld );
      utl::runtimeError( _renderpass.operator bool(), "failed to create render pass!" );
      utl::log( "create render pass succeeded." );
   }
   void
   Pipeline::createPipeline( const image::Extent2D& extent )
   {
      // ==== shader stage ============================================

      BasicShaders shaders {};
      shaders.createModules( _device, _dld );
      auto shader_stages { shaders.getShaderStages() };
      auto bind_desc { std::array { shaders.getVertInBindDesc() } };
      auto attrib_desc { std::array { shaders.getVertInAttribDesc() } };

      input_stage::VertexInputStateCreateInfo vertex_input_info( {}, bind_desc, attrib_desc );
      input_stage::AssemblyStateCreateInfo input_assembly( {}, pipeline::PrimitiveTopology::eTriangleList, vk::False );

      // ==== viewport , scissor ==================================================
      // NOTE: viewport は　表示される領域. scissor は　描画される領域.
      const float                          width { static_cast<float>( extent.width ) };
      const float                          height { static_cast<float>( extent.height ) };
      framebuffer::Viewport                viewport( .0f, .0f, width, height, .0f, 1.0f );
      framebuffer::Rect2D                  scissor( { 0, 0 }, extent );
      framebuffer::ViewportStateCreateInfo viewport_state { {}, 1, {}, 1, {} };
      // ==== dynamic state =======================================================
      // NOTE: 動的に処理したいパイプラインの処理を指定
      std::vector<pipeline::DynamicState> dynamic_states { pipeline::DynamicState::eViewport,
                                                           pipeline::DynamicState::eScissor,
                                                           pipeline::DynamicState::ePolygonModeEXT };
      pipeline::DynamicStateCreateInfo    dynamic_state( {}, dynamic_states );
      // ==== rasterization state =================================================
      pipeline::RasterrizationStateCreateInfo rasterizer(
        {},
        vk::False,    // depthClampEnable : trueの場合　指定したnear
                      // farを超えるフラグメントは破棄せず固定される。　主にシャドウマップなどで役立つ。
        vk::False,    // discardEnable : trueの場合
                      // ラスタライザーステージを通過しない。　フレームバッファへの出力が無効になる。
        {},    // polygonMode : フラグメントの生成方法を選択する fill, line, point
        pipeline::CullModeFlagBits::eBack,    // cullMode : カリングの種類を決定 back, front back and front, none
        pipeline::FrontFace::eClockwise,    // frontFace : 前面の決め方 時計回り、反時計。　Clookwise, CounterClookwise
        vk::
          False,                            // depthBiasEnable : trueの場合
                    // フラグメントの傾斜に基づいてバイアスをかけ、深度値を変更することが可能　シャドウマップなどで使用。
        .0f,    // depthBiasConstantFactor : depthBiasに基づく　オプション値
        .0f,    // depthBiasClamp : depthBiasに基づく　オプション値
        .0f,    // depthBiasSlopeFactor : depthBiasに基づく　オプション値
        1.0f    // lineWidth
      );
      // ==== multisample state ====================================================
      pipeline::MultisampleStateCreateInfo multisampleing(
        {}, pipeline::SampleCountFlagBits::e1, vk::False, 1.0f, nullptr, vk::False, vk::False );
      // ==== depth stencil state ==================================================
      // ==== color brend state ====================================================
      pipeline::ColorBlendAttachmentState color_blend_attachment( vk::False );
      color_blend_attachment.setColorWriteMask(
        pipeline::ColorComponentFlagBits::eR | pipeline::ColorComponentFlagBits::eG
        | pipeline::ColorComponentFlagBits::eB | pipeline::ColorComponentFlagBits::eA );
      pipeline::ColorBlendStateCreateInfo color_blending(
        {}, vk::False, pipeline::LogicOp::eCopy, 1, &color_blend_attachment );
      // ==== pipeline layout ======================================================
      pipeline::LayoutCreateInfo pipeline_layout_info {};
      _pipelinelayout = _device->createPipelineLayoutUnique( pipeline_layout_info, nullptr, _dld );
      utl::runtimeError( _pipelinelayout.operator bool(), "faild to create pipline layout!" );
      utl::log( "create pipeline layout succeeded." );
      // ==== graphics pipeline ====================================================
      pipeline::CreateInfo pipeline_info( {},
                                          shader_stages,
                                          &vertex_input_info,
                                          &input_assembly,
                                          {},
                                          &viewport_state,
                                          &rasterizer,
                                          &multisampleing,
                                          {},
                                          &color_blending,
                                          &dynamic_state,
                                          _pipelinelayout.get(),
                                          _renderpass.get(),
                                          0u,
                                          {},
                                          {} );

      auto result { _device->createGraphicsPipelinesUnique( nullptr, pipeline_info, nullptr, _dld ) };
      utl::runtimeError( ( result.result == vk::Result::eSuccess ), "failed to create pipeline!" );
      _pipeline = std::move( result.value[ 0 ] );
      utl::log( "create pipeline succeeded." );
   }
   Pipeline::Pipeline( const device::UniqueHandle& device, const DispatchLoaderDynamic& dld )
     : _device { device }
     , _dld { dld }
     , _pipelinelayout {}
     , _renderpass {}
     , _pipeline {}
     , pipelinelayout { _pipelinelayout }
     , renderpass { _renderpass }
     , pipeline { _pipeline }
   {}
   Pipeline::~Pipeline() {}
}    // namespace my_library::vkm