#pragma once
#include <atlbase.h>
//
#include "../Include/dxc/dxcapi.h"
#include "./spirv_cross/spirv_cross.hpp"
namespace my_library::shader
{
   enum class Type
   {
      VERTEX,
      PIXEL,
      COUNT
   };
   struct ShaderSrc
   {
      std::wstring fullpath;    // optional
      std::wstring directory;
      std::wstring filename;
      std::wstring extension;
      std::wstring entry_point;
      Type         type;
      ShaderSrc();
      ShaderSrc( const std::wstring& _fullpath, const std::wstring& _entry_point, const Type& type );
   };
   class HLSLCompiler
   {
   public:
      HLSLCompiler();
      ~HLSLCompiler();
      const std::wstring& out_directory;
      void
      compileToSPIR_V( const ShaderSrc& shader_src );

   private:
      std::wstring _out_dir;

      CComPtr<IDxcCompiler3> _compiler;
      CComPtr<IDxcUtils>     _utils;
   };

   class SpirvReflector : public spirv_cross::Compiler
   {
      using Parent = spirv_cross::Compiler;

   public:
      SpirvReflector( std::vector<uint32_t>& ir );
      SpirvReflector( std::vector<char>& ir );
      ~SpirvReflector();
      std::string
      getEntryPointName();

   private:
   };
}    // namespace my_library::shader
