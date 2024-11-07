#include "basicShader.hpp"

#include "../pch.hpp"
#include "../vertex.hpp"
#include "shader.hpp"

namespace my_library
{
   BasicShaders::BasicShaders()
     : _vs { std::make_unique<shader::ShaderForVK>(
       L"shaderSrc/VertexShader2.hlsl", L"main", vkm::shader_stage::FlagBits::eVertex ) }
     , _ps { std::make_unique<shader::ShaderForVK>(
         L"shaderSrc/PixelShader.hlsl", L"main", vkm::shader_stage::FlagBits::eFragment ) }
   {}

   BasicShaders::~BasicShaders() {}

   void
   my_library::BasicShaders::createModules( const vkm::device::UniqueHandle&  device,
                                            const vkm::DispatchLoaderDynamic& dld )
   {
      _vs->createModule( device, dld );
      _ps->createModule( device, dld );
   }

   const std::array<vkm::shader_stage::CreateInfo, 2>
   BasicShaders::getShaderStages()
   {
      return std::array<vkm::shader_stage::CreateInfo, 2> { _vs->getPipelineShaderStageCreateInfo(),
                                                            _ps->getPipelineShaderStageCreateInfo() };
   }

   const vkm::input_stage::BindingDescription
   BasicShaders::getVertInBindDesc()
   {
      return vkm::input_stage::BindingDescription(
        0, sizeof( vertex::BasicLayout ), vkm::input_stage::VertexInputRate::eVertex );
   }

   const std::array<vkm::input_stage::AttributeDescription, 2>
   BasicShaders::getVertInAttribDesc()
   {

      return std::array<vkm::input_stage::AttributeDescription, 2> {
         vkm::input_stage::AttributeDescription(
           0, 0, vkm::image::Format::eR32G32Sfloat, offsetof( vertex::BasicLayout, position ) ),
         vkm::input_stage::AttributeDescription(
           1, 0, vkm::image::Format::eR32G32B32Sfloat, offsetof( vertex::BasicLayout, color ) )
      };
   }
}    // namespace my_library
