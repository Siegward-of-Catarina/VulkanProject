#include "shader.hpp"

#include "../pch.hpp"
#include "../utilities.hpp"
#include "./spirv_cross/spirv_cross.hpp"
namespace
{
   namespace vkm    = my_library::vkm;
   using ShaderType = my_library::shader::Type;

   enum class ExtensionType
   {
      HLSL,
      GLSL,
      COUNT
   };

#ifdef _DEBUG
   constexpr bool COMPILE { true };
#else
   constexpr bool COMPILE { false };
#endif
}    // namespace
namespace
{
   ShaderType
   convertType( const vkm::shader_stage::FlagBits& type )
   {
      ShaderType out_type;
      switch ( type )
      {
         case vkm::shader_stage::FlagBits::eVertex : out_type = ShaderType::VERTEX; break;
         case vkm::shader_stage::FlagBits::eFragment : out_type = ShaderType::PIXEL; break;
      }
      return out_type;
   }
}    // namespace
namespace my_library::shader
{
   void
   ShaderForVK::createModule( const vkm::device::UniqueHandle& device, const vkm::DispatchLoaderDynamic& dld )
   {
      vkm::shader_module::CreateInfo info( {}, _shader_code.size(), reinterpret_cast<uint32_t*>( _shader_code.data() ) );
      _module = device->createShaderModuleUnique( info, nullptr, dld );

      utl::runtimeError( _module.operator bool(), "failed to create unique shader module!" );
      utl::log( "create shader module succeeded." );
   }
   const vkm::shader_stage::CreateInfo
   ShaderForVK::getPipelineShaderStageCreateInfo() const
   {
      return vkm::shader_stage::CreateInfo(
        {}, _shader_stage_flag_bits, _module.get(), _entry_point_name.c_str() );
   }
   ShaderForVK::ShaderForVK( const std::wstring&             shader_file,
                             const std::wstring&             entry_point,
                             const vkm::shader_stage::FlagBits& type )
     : _module {}
     , _shader_code {}
     , _entry_point_name {}
     , _shader_stage_flag_bits { type }
     , shader_module { _module }
     , entry_point_name { _entry_point_name }
   {
      ShaderSrc shader_src { shader_file, entry_point, convertType( type ) };
      // 現在はデバッグ時のみコンパイル
      //  TODO:シェーダーファイルに更新があったときのみコンパイルできるようにしたい
      shader::HLSLCompiler compiler;
      if ( COMPILE ) { compiler.compileToSPIR_V( shader_src ); }
      _shader_code = utl::file::readBinary<char>( compiler.out_directory + shader_src.filename + L".spv" );

      SpirvReflector reflector { _shader_code };
      _entry_point_name = reflector.getEntryPointName();
      //今はいったんいらないかな
      //auto res          = reflector.get_shader_resources();
   }
   ShaderForVK::~ShaderForVK() {}
}    // namespace my_library::shader
