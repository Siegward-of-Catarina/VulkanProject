#pragma once
#include <type_traits>
namespace my_library::enumExtensions
{
   template<typename E, typename std::enable_if_t<std::is_enum_v<E>, std::nullptr_t> = nullptr>
   class BitFlag final
   {
   private:
      using BaseType = typename std::underlying_type<E>::type;
      static constexpr BaseType
      cast( const E enum_value )
      {
         return static_cast<BaseType>( enum_value );
      }
      static constexpr E
      cast( const BaseType base_value )
      {
         return static_cast<E>( base_value );
      }

   public:
      constexpr BitFlag( const E enum_value ) noexcept : _value { cast( enum_value ) } {};
      constexpr BitFlag() noexcept : _value {} {};
      constexpr BitFlag( const std::vector<E>& enum_values ) noexcept : _value {}
      {
         for ( E e_val : enum_values ) { _value |= cast( e_val ); }
      }

      friend constexpr BitFlag
      operator|( const BitFlag lhs, const E rhs )
      {
         return BitFlag( cast( lhs.value() | cast( rhs ) ) );
      }
      friend constexpr BitFlag
      operator|( const E lhs, const BitFlag rhs )
      {
         return BitFlag( cast( cast( lhs ) | rhs.value() ) );
      }
      friend constexpr BitFlag
      operator|( const BitFlag lhs, const BitFlag rhs )
      {
         return BitFlag( cast( lhs.value() | rhs.value() ) );
      }

      friend constexpr BitFlag
      operator&( const BitFlag lhs, const E rhs )
      {
         return BitFlag( cast( lhs.value() & cast( rhs ) ) );
      }
      friend constexpr BitFlag
      operator&( const E lhs, const BitFlag rhs )
      {
         return BitFlag( cast( cast( lhs ) & rhs.value() ) );
      }
      friend constexpr BitFlag
      operator&( const BitFlag lhs, const BitFlag rhs )
      {
         return BitFlag( cast( lhs.value() & rhs.value() ) );
      }

      friend constexpr bool
      operator==( const BitFlag lhs, const E rhs )
      {
         return lhs.get() == rhs;
      }
      friend constexpr bool
      operator==( const E lhs, const BitFlag rhs )
      {
         return lhs == rhs.get();
      }
      friend constexpr bool
      operator==( const BitFlag lhs, const BitFlag rhs )
      {
         return lhs.get() == rhs.get();
      }

      friend constexpr bool
      operator!=( const BitFlag lhs, const E rhs )
      {
         return lhs.get() != rhs;
      }
      friend constexpr bool
      operator!=( const E lhs, const BitFlag rhs )
      {
         return lhs != rhs.get();
      }
      friend constexpr bool
      operator!=( const BitFlag lhs, const BitFlag rhs )
      {
         return lhs.get() != rhs.get();
      }

      constexpr BitFlag
      operator~() const
      {
         return BitFlag( cast( ~_value ) );
      }

      constexpr E
      get() const
      {
         return cast( _value );
      }

      constexpr bool
      validFlag( const E enum_value ) const
      {
         return this->_value & cast( enum_value );
      }

      BitFlag&
      operator|=( const E rhs )
      {
         return _value |= cast( rhs ), *this;
      }
      BitFlag&
      operator&=( const E rhs )
      {
         return _value &= cast( rhs ), *this;
      }

   private:
      BaseType _value;
      constexpr BaseType
      value() const
      {
         return _value;
      }
   };

   template<typename E, typename std::enable_if_t<std::is_enum_v<E>, std::nullptr_t> = nullptr>
   constexpr BitFlag<E>
   operator|( const E lhs, const E rhs )
   {
      using BaseType = typename std::underlying_type<E>::type;
      return BitFlag<E>( static_cast<E>( static_cast<BaseType>( lhs ) | static_cast<BaseType>( rhs ) ) );
   }
   template<typename E, typename std::enable_if_t<std::is_enum_v<E>, std::nullptr_t> = nullptr>
   constexpr BitFlag<E>
   operator&( const E lhs, const E rhs )
   {
      using BaseType = typename std::underlying_type<E>::type;
      return BitFlag<E>( static_cast<E>( static_cast<BaseType>( lhs ) & static_cast<BaseType>( rhs ) ) );
   }
   template<typename E, typename std::enable_if_t<std::is_enum_v<E>, std::nullptr_t> = nullptr>
   constexpr BitFlag<E>
   operator~( const E enum_value )
   {
      using BaseType = typename std::underlying_type<E>::type;
      return BitFlag<E>( static_cast<E>( ~static_cast<BaseType>( enum_value ) ) );
   }
}    // namespace my_library::enumExtensions
