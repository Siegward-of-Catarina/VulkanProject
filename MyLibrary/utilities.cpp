#include "utilities.hpp"

#include "pch.hpp"
namespace my_library::utl
{
   void
   runtimeError( const std::string& error, const std::source_location location )
   {
      std::string s { location.file_name() };
      uint32_t i { static_cast<uint32_t>( s.rfind( "\\" ) ) + 1 };    // 見つかった位置の次が欲しいので+1

      std::string error_msg {
         error + " : This code is on [line:" + std::to_string( location.line() )
         + "], in [file:" + std::string {s.begin() + i, s.end()}
             + "]."
      };

      throw std::runtime_error( error_msg );
   }
   size_t
   align( const size_t& align, const size_t& s )
   {
      return ( s + align - 1 ) / align * align;
   }
}    // namespace my_library::utl

namespace my_library::utl::file
{
   std::wstring
   getExePath()
   {
      wchar_t* exe_path { new wchar_t[ MAX_PATH ] };
#if ( _WIN32 )
      GetModuleFileNameW( NULL, exe_path, MAX_PATH );
#endif
      std::wstring out { exe_path, exe_path + MAX_PATH - 1 };
      return splitToDirectoryPath( out );
   }
   const std::wstring
   splitToExtension( const std::wstring& file )
   {
      const size_t begin { ( file.rfind( '.' ) != std::wstring::npos ) ? file.rfind( '.' ) + 1 : 0 };
      const size_t count { file.size() - begin };
      return file.substr( begin, count );
   }
   const std::string
   splitToExtension( const std::string& file )
   {
      const size_t begin { ( file.rfind( '.' ) != std::wstring::npos ) ? file.rfind( '.' ) + 1 : 0 };
      const size_t count { file.size() - begin };
      return file.substr( begin, count );
   }
   const std::wstring
   splitToFileName( const std::wstring& file )
   {
      size_t begin { 0 };
      if ( file.rfind( '\\' ) != std::wstring::npos ) begin = file.rfind( '\\' ) + 1;
      if ( file.rfind( '/' ) != std::wstring::npos ) begin = file.rfind( '/' ) + 1;

      const size_t count { ( file.rfind( '.' ) != std::wstring::npos ) ? file.rfind( '.' ) - begin
                                                                       : file.size() - begin };
      return file.substr( begin, count );
   }
   const std::string
   splitToFileName( const std::string& file )
   {
      size_t begin { 0 };
      if ( file.rfind( '\\' ) != std::string::npos ) begin = file.rfind( '\\' ) + 1;
      if ( file.rfind( '/' ) != std::string::npos ) begin = file.rfind( '/' ) + 1;

      const size_t count { ( file.rfind( '.' ) != std::string::npos ) ? file.rfind( '.' ) - begin
                                                                      : file.size() - begin };
      return file.substr( begin, count );
   }
   const std::wstring
   splitToDirectoryPath( const std::wstring& file )
   {
      const size_t begin { 0 };
      // sizeは maxindex + 1 なので　「 file.rfind( '/' ) + 1 」
      size_t count { file.size() - 1 };
      if ( file.rfind( '\\' ) != std::wstring::npos ) count = ( file.rfind( '\\' ) + 1 );
      if ( file.rfind( '/' ) != std::wstring::npos ) count = ( file.rfind( '/' ) + 1 );
      return file.substr( begin, count );
   }
   const std::string
   splitToDirectoryPath( const std::string& file )
   {
      const size_t begin { 0 };
      // sizeは maxindex + 1 なので　「 file.rfind( '/' ) + 1 」
      size_t count { file.size() - 1 };
      if ( file.rfind( '\\' ) != std::string::npos ) count = ( file.rfind( '\\' ) + 1 );
      if ( file.rfind( '/' ) != std::string::npos ) count = ( file.rfind( '/' ) + 1 );
      return file.substr( begin, count );
   }
}    // namespace my_library::utl::file

#if ( _WIN32 )
namespace my_library::utl::win
{
   std::string
   getErrorMessage( DWORD id )
   {
      static_assert( _WIN32, "this function is windows only." );
      std::string out;
      LPVOID      buffer { NULL };
      DWORD       result { FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                                       | FORMAT_MESSAGE_IGNORE_INSERTS,
                                     NULL,
                                     id,
                                     MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
                                     reinterpret_cast<char*>( buffer ),
                                     0,
                                     NULL ) };
      if ( result > 0 && buffer != NULL )
      {
         out = reinterpret_cast<char*>( buffer );
         LocalFree( buffer );
         // 末尾改行を削除
         assert( !out.empty() );
         if ( out[ out.size() - 1 ] == L'\n' ) out.erase( out.size() - 1 );
         if ( out[ out.size() - 1 ] == L'\r' ) out.erase( out.size() - 1 );
      }
      return out;
   }
   namespace string
   {
      const std::wstring
      stringToWString( const std::string& str )
      {
         static_assert( sizeof( wchar_t ) == 2, "this function is windows only." );
         // wchar 変換後のバイト数を取得
         const int buffer_size { MultiByteToWideChar( CP_ACP, 0, str.c_str(), -1, nullptr, 0 ) };
         // バッファを確保
         wchar_t* w_str { new wchar_t[ buffer_size ] };
         // wchar に変換
         if ( !MultiByteToWideChar( CP_ACP, 0, str.c_str(), -1, w_str, buffer_size ) )
            utl::runtimeError( getLastErrorMessage() );
         // wstring作成
         std::wstring out { w_str, w_str + buffer_size - 1 };
         // バッファ開放
         delete[] w_str;

         return out;
      }
      const std::string
      wstringToString( const std::wstring& wstr )
      {
         static_assert( sizeof( wchar_t ) == 2, "this function is windows only." );
         // wchar 変換後のバイト数を取得
         const int buffer_size { WideCharToMultiByte( CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr ) };
         // バッファを確保
         char* str { new char[ buffer_size ] };
         // wchar に変換
         if ( !WideCharToMultiByte( CP_ACP, 0, wstr.c_str(), -1, str, buffer_size, nullptr, nullptr ) )
            utl::runtimeError( getLastErrorMessage() );
         // wstring作成
         std::string out { str, str + buffer_size - 1 };
         // バッファ開放
         delete[] str;

         return out;
      }
   }    // namespace string
}    // namespace my_library::utl::win

#endif