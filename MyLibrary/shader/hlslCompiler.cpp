#include "hlslCompiler.hpp"

#include "../pch.hpp"
#include "../utilities.hpp"

#include <filesystem>
#include <fstream>
namespace
{
   using shaderType = my_library::shader::Type;
   namespace utl    = my_library::utl;
}    // namespace
namespace
{
   LPCWSTR
   selectProfile( const shaderType& type )
   {
      LPCWSTR target_profile {};
      switch ( type )
      {
         case shaderType::VERTEX : target_profile = L"vs_6_4"; break;
         case shaderType::PIXEL : target_profile = L"ps_6_4"; break;
      }
      return target_profile;
   }
   CComPtr<IDxcBlobEncoding>
   loadHLSL( const CComPtr<IDxcUtils>& _utils, const std::wstring& shader_file )
   {
      HRESULT hres {};

      std::uint32_t             codepage { DXC_CP_ACP };
      CComPtr<IDxcBlobEncoding> source_blob;
      hres = _utils->LoadFile( shader_file.c_str(), &codepage, &source_blob );
      utl::win::runtimeError( hres, "Could not load shader file!" );
      return source_blob;
   }
   CComPtr<IDxcResult>
   compile( const CComPtr<IDxcCompiler3>&    _compiler,
            std::vector<LPCWSTR>&            arguments,
            const CComPtr<IDxcBlobEncoding>& source_blob )
   {
      DxcBuffer buffer {};
      buffer.Encoding = DXC_CP_ACP;
      buffer.Ptr      = source_blob->GetBufferPointer();
      buffer.Size     = source_blob->GetBufferSize();

      CComPtr<IDxcResult> compile_res { nullptr };

      HRESULT hres { _compiler->Compile(
        &buffer, arguments.data(), static_cast<uint32_t>( arguments.size() ), nullptr, IID_PPV_ARGS( &compile_res ) ) };

      if ( SUCCEEDED( hres ) ) compile_res->GetStatus( &hres );

      if ( FAILED( hres ) && ( compile_res ) )
      {
         CComPtr<IDxcBlobEncoding> error_blob;
         hres = compile_res->GetErrorBuffer( &error_blob );
         if ( SUCCEEDED( hres ) && error_blob )
         {
            std::cerr << "Shader compilation failed : \n\n"
                      << static_cast<const char*>( error_blob->GetBufferPointer() );
            utl::runtimeError( "Compilation failed!" );
         }
      }

      return compile_res;
   }
   void
   outShaderFile( const CComPtr<IDxcResult>& compile_res, const std::wstring out_path, const std::wstring filename )
   {
      // ディレクトリが存在するか確認し、なければ作成
      namespace fs = std::filesystem;
      fs::path dir = out_path;
      if ( !fs::exists( dir ) ) fs::create_directory( dir );

      CComPtr<IDxcBlob> code { nullptr };
      compile_res->GetResult( &code );
      std::ofstream outf( ( out_path + filename ), std::ios::out | std::ios::binary );
      outf.write( static_cast<char*>( code->GetBufferPointer() ), code->GetBufferSize() );
      outf.close();
   }
}    // namespace
namespace my_library::shader
{
   HLSLCompiler::HLSLCompiler() : _out_dir { utl::file::getExePath() + L"spir_v\\" }, out_directory { _out_dir }
   {
      // dxc compiler, utils 初期化
      HRESULT hres {};
      hres = DxcCreateInstance( CLSID_DxcUtils, IID_PPV_ARGS( &_utils ) );
      utl::win::runtimeError( hres, "Could not init DXC Utiliy!" );

      hres = DxcCreateInstance( CLSID_DxcCompiler, IID_PPV_ARGS( &_compiler ) );
      utl::win::runtimeError( hres, "Could not init DXC Compiler!" );
   }
   HLSLCompiler::~HLSLCompiler() {}
   void
   HLSLCompiler::compileToSPIR_V( const ShaderSrc& shader_src )
   {
      // hlslファイルじゃない場合例外
      utl::runtimeError( shader_src.extension == L"hlsl", "this shader file is not hlsl file!" );

      HRESULT                   hres {};
      std::filesystem::current_path( "../MyLibrary" );
      auto absolute_path { std::filesystem::current_path().generic_wstring() + L'/' + shader_src.fullpath };

      CComPtr<IDxcBlobEncoding> source_blob { ::loadHLSL( _utils, absolute_path ) };
      LPCWSTR                   target_profile { ::selectProfile( shader_src.type ) };
      std::vector<LPCWSTR>      arguments {
         shader_src.fullpath.c_str(), L"-E", shader_src.entry_point.c_str(), L"-T", target_profile, L"-spirv"
      };

      // コンパイル
      CComPtr<IDxcResult> compil_res { ::compile( _compiler, arguments, source_blob ) };

      // Get compilation result
      ::outShaderFile( compil_res, _out_dir, shader_src.filename + L".spv" );
   }

   // shader src
   ShaderSrc::ShaderSrc() : fullpath {}, directory {}, filename {}, extension {}, entry_point {}, type { Type::VERTEX }
   {}
   ShaderSrc::ShaderSrc( const std::wstring& _fullpath, const std::wstring& _entry_point, const Type& type )
     : fullpath { _fullpath }
     , directory { utl::file::splitToDirectoryPath( _fullpath ) }
     , filename { utl::file::splitToFileName( _fullpath ) }
     , extension { utl::file::splitToExtension( _fullpath ) }
     , entry_point { _entry_point }
     , type { type }
   {}

}    // namespace my_library::shader
namespace my_library::shader
{
   SpirvReflector::SpirvReflector( std::vector<uint32_t>& ir ) : Parent { ir } {}
   SpirvReflector::SpirvReflector( std::vector<char>& ir )
     : Parent { reinterpret_cast<uint32_t*>( ir.data() ), ir.size() / sizeof( uint32_t ) }
   {}
   SpirvReflector::~SpirvReflector() {}
   std::string
   SpirvReflector::getEntryPointName()
   {
      auto entry_pt_data = Parent::get_entry_point();
      return entry_pt_data.name;
   }
}    // namespace my_library::shader