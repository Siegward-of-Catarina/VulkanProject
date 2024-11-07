#pragma once
#include <fstream>
#include <iostream>
#include <source_location>
#include <string>
#include <type_traits>
#if ( _WIN32 )
#include <windows.h>
#elif ( APPLE )
#include <mach-o/dyld.h>    //mac os x ?
#endif
namespace my_library::utl
{
   //==== logger ======================================
#ifdef NDEBUG
   template<typename out>
   void
   log( out& o )
   {
      std::cout << o << std::endl;
   }
#else
   template<typename Out>
   void
   log( const Out& out, const std::source_location location = std::source_location::current() )
   {
      std::string s { location.file_name() };
      uint32_t i { static_cast<uint32_t>( s.rfind( "\\" ) ) + 1 };    // 見つかった位置の次が欲しいので+1
      std::cout << out << " : This code is on [line:" << location.line()
                << "], in [file:" << std::string { s.begin() + i, s.end() } << "]. " << std::endl;
   }
#endif
   //==== runtime error ================================
   void
   runtimeError( const std::string& error, const std::source_location location = std::source_location::current() );
   inline void
   runtimeError( const bool&                expression,
                 const std::string&         error_str,
                 const std::source_location location = std::source_location::current() )
   {
      if ( expression == false ) runtimeError( error_str, location );
   }

   size_t
   align( const size_t& align, const size_t& s );
   namespace file
   {
      template<typename T>
      std::vector<T>
      readBinary( const std::string& filename )
      {
         // ate : 末尾から読み取り
         std::ifstream file( filename.c_str(), std::ios::ate | std::ios::binary );
         runtimeError( file.is_open(), "failed to open file!" );
         // ストリームバッファから現在の位置を取得
         size_t         file_size = static_cast<size_t>( file.tellg() );
         std::vector<T> buffer( align( sizeof( T ), file_size ) / sizeof( T ) );

         file.seekg( 0 );    // 先頭に戻る
         file.read( buffer.data(), file_size );

         file.close();
         return buffer;
      }
      template<typename T>
      std::vector<T>
      readBinary( const std::wstring& filename )
      {
         // ate : 末尾から読み取り
         std::ifstream file( filename, std::ios::ate | std::ios::binary );
         runtimeError( file.is_open(), "failed to open file!" );
         // ストリームバッファから現在の位置を取得
         size_t         file_size = static_cast<size_t>( file.tellg() );
         std::vector<T> buffer( align( sizeof( T ), file_size ) / sizeof( T ) );

         file.seekg( 0 );    // 先頭に戻る
         file.read( reinterpret_cast<char*>( buffer.data() ), file_size );

         file.close();
         return buffer;
      }
      std::wstring
      getExePath();
      const std::wstring
      splitToExtension( const std::wstring& file );
      const std::string
      splitToExtension( const std::string& file );
      const std::wstring
      splitToFileName( const std::wstring& file );
      const std::string
      splitToFileName( const std::string& file );
      const std::wstring
      splitToDirectoryPath( const std::wstring& file );
      const std::string
      splitToDirectoryPath( const std::string& file );
   }    // namespace file
   namespace my_enum
   {
      /// <summary>
      /// enum class 作成時に指定したベースタイプに変換する
      /// </summary>
      /// <typeparam name="Enum"></typeparam>
      /// <param name="e"></param>
      /// <returns>static_cast＜ base type ＞(e) </returns>
      template<typename Enum>
      typename std::underlying_type<Enum>::type
      e_to_b( Enum e )
      {
         static_assert( std::is_enum<Enum>::value, "e_to_b() requires enum argument" );
         return static_cast<typename std::underlying_type<Enum>::type>( e );
      }
   };    // namespace my_enum

         //==== platfrom ============================
#if ( _WIN32 )
   namespace win
   {
      inline void
      runtimeError( const HRESULT&             result,
                    const std::string&         error_str,
                    const std::source_location location = std::source_location::current() )
      {
         if ( FAILED( result ) ) utl::runtimeError( error_str, location );
      }
      std::string
      getErrorMessage( DWORD id );
      inline std::string
      getLastErrorMessage()
      {
         static_assert( _WIN32, "this function is windows only." );
         return getErrorMessage( GetLastError() );
      }
      namespace string
      {
         const std::wstring
         stringToWString( const std::string& str );
         const std::string
         wstringToString( const std::wstring& wstr );
      }    // namespace string
   }       // namespace win
#endif
}    // namespace my_library::utl