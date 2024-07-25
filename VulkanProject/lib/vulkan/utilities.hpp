#pragma once
#include <iostream>
#include <source_location>
#include <string>
#include <type_traits>
namespace my_library::utl
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

   template<typename out>
   void
   log( out& o, const std::source_location location = std::source_location::current() )
   {
      std::string s { location.file_name() };
      uint16_t i = static_cast<uint16_t>( s.rfind( "\\" ) ) + 1;    // 見つかった位置の次が欲しいので+1
      std::cout << o << " : This code is on [line:" << location.line() << "], in [file:"
                << std::string { s.begin() + i, s.end() } << "]. " << std::endl;
   }
}    // namespace my_library::utl