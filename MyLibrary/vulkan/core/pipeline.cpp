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
        format,                                    // format : �X���b�v�`�F�C���ƈ�v������
        pipeline::SampleCountFlagBits::e1,         // samples : �}���`�T���v�����O�p
        render_pass::AttachmentLoadOp::eClear,     // loadOp : �����_�����O�O�Ƀf�[�^�̈����������߂�
        render_pass::AttachmentStoreOp::eStore,    // storeOp : �����_�����O��̂Ł[���̈����������߂�
        render_pass::AttachmentLoadOp::eDontCare,     // stencilLoadOp : �X�e���V���o�b�t�@�ɑ΂��� loadOp
        render_pass::AttachmentStoreOp::eDontCare,    // stencilStoreOp : �X�e���V���o�b�t�@�ɑ΂��� storeOp
        render_pass::ImageLayout::eUndefined,         // initiallayout : �p�X�J�n�O�̃s�N�Z�����C�A�E�g
        render_pass::ImageLayout::ePresentSrcKHR      // finalLayout ; �p�X�I����̃s�N�Z�����C�A�E�g
      );

      // ==== sub pass =============
      render_pass::AttachmentReference color_attachment_ref(
        0,    // attachment : attachmentDescription�z��̃C���f�b�N�X
        render_pass::ImageLayout::eColorAttachmentOptimal    // layout : attachment�̎g�p���@
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
      // NOTE: viewport �́@�\�������̈�. scissor �́@�`�悳���̈�.
      const float                          width { static_cast<float>( extent.width ) };
      const float                          height { static_cast<float>( extent.height ) };
      framebuffer::Viewport                viewport( .0f, .0f, width, height, .0f, 1.0f );
      framebuffer::Rect2D                  scissor( { 0, 0 }, extent );
      framebuffer::ViewportStateCreateInfo viewport_state { {}, 1, {}, 1, {} };
      // ==== dynamic state =======================================================
      // NOTE: ���I�ɏ����������p�C�v���C���̏������w��
      std::vector<pipeline::DynamicState> dynamic_states { pipeline::DynamicState::eViewport,
                                                           pipeline::DynamicState::eScissor,
                                                           pipeline::DynamicState::ePolygonModeEXT };
      pipeline::DynamicStateCreateInfo    dynamic_state( {}, dynamic_states );
      // ==== rasterization state =================================================
      pipeline::RasterrizationStateCreateInfo rasterizer(
        {},
        vk::False,    // depthClampEnable : true�̏ꍇ�@�w�肵��near
                      // far�𒴂���t���O�����g�͔j�������Œ肳���B�@��ɃV���h�E�}�b�v�ȂǂŖ𗧂B
        vk::False,    // discardEnable : true�̏ꍇ
                      // ���X�^���C�U�[�X�e�[�W��ʉ߂��Ȃ��B�@�t���[���o�b�t�@�ւ̏o�͂������ɂȂ�B
        {},    // polygonMode : �t���O�����g�̐������@��I������ fill, line, point
        pipeline::CullModeFlagBits::eBack,    // cullMode : �J�����O�̎�ނ����� back, front back and front, none
        pipeline::FrontFace::eClockwise,    // frontFace : �O�ʂ̌��ߕ� ���v���A�����v�B�@Clookwise, CounterClookwise
        vk::
          False,                            // depthBiasEnable : true�̏ꍇ
                    // �t���O�����g�̌X�΂Ɋ�Â��ăo�C�A�X�������A�[�x�l��ύX���邱�Ƃ��\�@�V���h�E�}�b�v�ȂǂŎg�p�B
        .0f,    // depthBiasConstantFactor : depthBias�Ɋ�Â��@�I�v�V�����l
        .0f,    // depthBiasClamp : depthBias�Ɋ�Â��@�I�v�V�����l
        .0f,    // depthBiasSlopeFactor : depthBias�Ɋ�Â��@�I�v�V�����l
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