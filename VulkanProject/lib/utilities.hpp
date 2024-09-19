#pragma once
#include <iostream>
#include <source_location>
#include <string>
#include <type_traits>
namespace my_library::utl
{
   namespace my_enum
   {
      /// <summary>
      /// enum class �쐬���Ɏw�肵���x�[�X�^�C�v�ɕϊ�����
      /// </summary>
      /// <typeparam name="Enum"></typeparam>
      /// <param name="e"></param>
      /// <returns>static_cast�� base type ��(e) </returns>
      template<typename Enum>
      typename std::underlying_type<Enum>::type
      e_to_b( Enum e )
      {
         static_assert( std::is_enum<Enum>::value, "e_to_b() requires enum argument" );
         return static_cast<typename std::underlying_type<Enum>::type>( e );
      }
   };    // namespace my_enum

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
      uint32_t i { static_cast<uint32_t>( s.rfind( "\\" ) ) + 1 };    // ���������ʒu�̎����~�����̂�+1
      std::cout << out << " : This code is on [line:" << location.line()
                << "], in [file:" << std::string { s.begin() + i, s.end() } << "]. " << std::endl;
   }
#endif
   inline void
   runtimeError( const std::string& error, const std::source_location location = std::source_location::current() )
   {
      std::string s { location.file_name() };
      uint32_t i { static_cast<uint32_t>( s.rfind( "\\" ) ) + 1 };    // ���������ʒu�̎����~�����̂�+1

      std::string error_msg {
         error + " : This code is on [line:" + std::to_string( location.line() )
         + "], in [file:" + std::string {s.begin() + i, s.end()}
             + "]."
      };

      throw std::runtime_error( error_msg );
   }

}    // namespace my_library::utl