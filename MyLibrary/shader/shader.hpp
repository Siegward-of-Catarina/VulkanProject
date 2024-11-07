#pragma once
#include "../vulkan/vkObjectTypes.hpp"
#include "hlslCompiler.hpp"
namespace my_library::shader
{
   class ShaderForVK
   {
   public:
      void
      createModule( const vkm::device::UniqueHandle& device, const vkm::DispatchLoaderDynamic& dld );
      const vkm::shader_stage::CreateInfo
                                     getPipelineShaderStageCreateInfo() const;
      const vkm::shader_module::UniqueHandle& shader_module;
      const std::string&             entry_point_name;
      ShaderForVK( const std::wstring&             shader_file,
                   const std::wstring&             entry_point,
                   const vkm::shader_stage::FlagBits& type );
      ~ShaderForVK();

   private:
      vkm::shader_module::UniqueHandle  _module;
      std::vector<char>    _shader_code;
      std::string              _entry_point_name;
      vkm::shader_stage::FlagBits _shader_stage_flag_bits;
   };
}    // namespace my_library::shader
