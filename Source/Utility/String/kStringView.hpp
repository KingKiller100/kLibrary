#pragma once

#include "../../HelperMacros.hpp"

#include "../Debug/kAssert.hpp"

#include "../../TypeTraits/CharacterTraits.hpp"
#include "kStringTricks.hpp"

#include <limits>
#include <string>
#include <xutility>
#include <cassert>

namespace klib
{
	namespace kString
	{
		template <class CharType, class Traits = type_trait::Traits<CharType>>
		class Template_String_View
		{
			template <typename T>
			class Iterator;

		public:
			using Size = size_t;
			using Type = CharType;
			using Ptr = CharType*;
			using Ref = CharType&;
			using Iter = Iterator<Template_String_View>;

			using Const_Ptr = const CharType*;
			using Const_Ref = const CharType&;
			using Const_Iter = const Iter;

			using Traits_t = Traits;

			enum : unsigned short
			{
				EQUAL = 0
				, MYSTR_SHORT
				, OTHER_STR_SHORT
				, DIFFERENT = 65535
			};

			static constexpr auto npos{static_cast<Size>( -1 )};
			static constexpr auto null_terminator = type_trait::g_NullTerminator<Type>;

			static_assert(std::is_same_v<CharType, typename Traits::char_type>,
				"Type entered into string_view class does not match a char type object (char, wchar_t, char32_t, etc)!");

			constexpr Template_String_View() noexcept
				: string( nullptr )
				, length( 0 )
			{}

			constexpr Template_String_View( const Template_String_View& ) noexcept = default;

			constexpr explicit Template_String_View( const Const_Ptr stringLiteral, const Size size, const Size offset = 0 ) noexcept
				: string( stringLiteral )
				, length( size )
			{
				CheckWithinLength( offset, "Offset greater than length" );
				string = string + offset;
				length -= offset;
			}

			constexpr Template_String_View( const Const_Ptr stringLiteral ) noexcept
				: string( stringLiteral )
			{
				length = GetStrLength( string );
			}

			constexpr Template_String_View( const std::basic_string<CharType>& str ) noexcept
				: string( str.c_str() )
				, length( str.size() )
			{ }

			constexpr Template_String_View& operator=( const Template_String_View& other ) noexcept
			{
				this->string = other.string;
				this->length = other.length;
				return *this;
			}

			constexpr Template_String_View& operator=( const std::basic_string<CharType>& other ) noexcept
			{
				this->string = other.c_str();
				this->length = other.size();
				return *this;
			}

			// Operators
			constexpr bool operator==( const CharType& item ) const
			{
				return Length() == 1 && *Data() == item;
			}

			constexpr bool operator==( const Template_String_View& other ) const
			{
				return Length() == other.length && other.Data() == string;
			}

			constexpr bool operator==( Const_Ptr str ) const
			{
				const Template_String_View temp( str );
				return Length() == temp.Length() && temp.Data() == string;
			}

			template <class Stringish, typename = std::enable_if_t<
				          type_trait::Is_String_V<Stringish>
			          >>
			constexpr bool operator==( const Stringish& str ) const
			{
				return ( str.size() == Length()
					&& str.data() == string );
			}

			template <typename ComparerT>
			constexpr bool operator!=( const ComparerT& other )
			{
				return !( *this == other );
			}

			//****************************************************************

			constexpr operator Const_Ptr() const
			{
				return string;
			}

			USE_RESULT constexpr Const_Ref operator[]( const Size index ) const noexcept
			{
				return at( index );
			}

			USE_RESULT constexpr Const_Ref at( const Size index ) const
			{
				CheckWithinLength( index,
					"Index must be within 0 and length of string" );

				return string[index];
			}

			USE_RESULT constexpr Const_Ptr Data() const
			{
				return string;
			}

			USE_RESULT constexpr Size Length() const
			{
				return length;
			}

			USE_RESULT constexpr bool Empty() const
			{
				return length == 0;
			}

			constexpr void Clear()
			{
				string = nullptr;
				length = 0;
			}

			USE_RESULT constexpr Const_Ref Front() const
			{
				assert( string != nullptr );
				return string[0];
			}

			USE_RESULT constexpr Const_Ref Back() const
			{
				assert( string != nullptr );
				return string[length - 1];
			}

			constexpr bool StartsWith( const CharType item ) noexcept
			{
				return !Empty() && Front() == item;
			}

			constexpr bool StartsWith( const Template_String_View& other ) noexcept
			{
				if ( other.length > length || Empty() )
					return false;

				auto currentChar = string;
				auto otherChar = other.string;

				while ( *otherChar != null_terminator )
				{
					if ( *currentChar != *otherChar )
						return false;

					++currentChar;
					++otherChar;
				}

				return true;
			}

			constexpr bool StartsWith( Const_Ptr const literal ) noexcept
			{
				return StartsWith( Template_String_View( literal ) );
			}

			constexpr bool EndsWith( const CharType item ) noexcept
			{
				return !Empty() && Back() == item;
			}

			constexpr bool EndsWith( const Template_String_View& other ) noexcept
			{
				if ( other.length > length )
					return false;

				auto lengthDiff = length - other.length;

				auto currentChar = string + lengthDiff;
				auto otherChar = other.string;

				while ( *otherChar != null_terminator )
				{
					if ( *currentChar != *otherChar )
						return false;

					++currentChar;
					++otherChar;
				}

				return true;
			}

			constexpr bool EndsWith( Const_Ptr const literal ) noexcept
			{
				return EndsWith( Template_String_View( literal ) );
			}


			constexpr void Swap( Template_String_View& other ) noexcept
			{
				const auto temp = other;
				other = *this;
				*this = temp;
			}

			USE_RESULT constexpr int Compare( const Template_String_View& other ) noexcept
			{
				// Write it yourself, can't rely on this to be in older versions of the STL
				auto myString = string;
				auto otherString = other.string;

				while ( *myString != null_terminator && *otherString != null_terminator )
				{
					if ( *myString == *otherString )
					{
						++myString;
						++otherString;
					}
					else
					{
						return DIFFERENT;
					}
				}

				if ( *myString == null_terminator && *otherString == null_terminator )
					return EQUAL;
				else if ( *myString == null_terminator )
					return MYSTR_SHORT;
				else
					return OTHER_STR_SHORT;
			}


			USE_RESULT constexpr Const_Ptr SubData( const Size amount, const Size offset = 0 ) const
			{
				CheckWithinLength( amount + offset,
					"Amount and offset provided give an index greater than the length of the string" );

				return Template_String_View( string + offset, amount ).Data();
			}

			USE_RESULT constexpr Template_String_View Substr( const Size amount, const Size offset = 0 ) const
			{
				CheckWithinLength( amount + offset,
					"Amount and offset provided give an index greater than the length of the string" );

				return Template_String_View( string + offset, amount );
			}

			constexpr Size FirstIndexOf( const CharType item, const Size offset = 0, Size searchLimit = ( std::numeric_limits<Size>::max )() )
			{
				CheckWithinLength( offset,
					"Offset is greater than the length of string" );

				auto currentChar = string + offset;
				Size count( offset );

				while ( *currentChar != null_terminator
					&& searchLimit > 0 )
				{
					if ( *currentChar == item )
						return count;

					++count;
					--searchLimit;
					++currentChar;
				}

				return npos;
			}

			constexpr Size FirstIndexOf( Const_Ptr search, const Size offset = 0 )
			{
				CheckWithinLength( offset,
					"Offset is greater than the length of string" );

				const auto index = kString::Find( string, search, offset );
				return index;
			}

			constexpr Size FirstIndexOf( const Template_String_View& other, const Size offset = 0 )
			{
				return FirstIndexOf( other.string, offset );
			}

			constexpr Size FirstIndexOfNot( const CharType item, const Size offset = 0 )
			{
				CheckWithinLength( offset,
					"Offset is greater than the length of string" );

				auto currentChar = string + offset;
				auto count = offset;
				while ( *currentChar != null_terminator )
				{
					if ( *currentChar != item )
						return count;

					++count;
					++currentChar;
				}

				return npos;
			}

			constexpr Size FirstIndexOfNot( Const_Ptr str, const Size offset = 0 )
			{
				CheckWithinLength( offset,
					"Offset is greater than the length of string" );

				auto currentChar = string + offset;
				auto compareStr = str;
				auto count = offset;
				Size idx = 0U;

				while ( *currentChar != null_terminator )
				{
					if ( *currentChar != compareStr[idx] )
						return count;

					++count;
					++currentChar;
					idx++;
				}

				return npos;
			}

			constexpr Size LastIndexOf( const CharType item, const Size offset = 0 )
			{
				CheckWithinLength( offset,
					"Offset greater than length of this string" );

				auto currentChar = string + length - offset - 1;
				Size pos( length - offset );

				while ( currentChar != string )
				{
					if ( *currentChar == item )
						return pos;

					--pos;
					--currentChar;
				}
				return npos;
			}

			constexpr Size LastIndexOfNot( const CharType item, const Size offset = 0 ) noexcept
			{
				auto currentChar = string + length - offset - 1;
				Size pos( length - offset );

				while ( currentChar != string )
				{
					if ( *currentChar != item )
						return pos;

					--pos;
					--currentChar;
				}
				return npos;
			}

			constexpr Size LastIndexOf( Const_Ptr str, const Size offset = 0 ) noexcept
			{
				CheckWithinLength( offset, "Offset greater than length of this string" );

				auto pos = LastIndexOf( str[0], offset );
				auto strLength = GetStrLength( str );

				if ( pos == npos )
					return npos;

				auto viewStr = string + length - pos;
				Size strIdx( 0 );

				while ( viewStr != string )
				{
					if ( *viewStr == str[strIdx] )
					{
						++viewStr;
						++pos;
						++strIdx;
						continue;
					}

					if ( str[strIdx] == null_terminator )
						return ( pos - strLength );

					strIdx = 0;
					pos = LastIndexOf( str[0], pos );
					viewStr = string + length - pos;
				}

				return npos;
			}

			void RemovePrefix( const Size offset )
			{
				CheckWithinLength( offset,
					"Offset provided is outside the range of the string" );

				string += offset;
				length -= offset;
			}

			Const_Ptr RemoveSuffix( const Size offset )
			{
				CheckWithinLength( offset,
					"Offset provided is outside the range of the string" );

				length -= offset;
				return string + offset;
			}

			USE_RESULT constexpr bool IsEqual( const Template_String_View& other ) const noexcept
			{
				if ( length != other.length )
					return false;

				auto thisCurrentChar = string;
				auto otherCurrentChar = other.string;

				while ( *thisCurrentChar == *otherCurrentChar )
				{
					++thisCurrentChar;
					++otherCurrentChar;

					if ( *thisCurrentChar == null_terminator
						&& *otherCurrentChar == null_terminator )
						return true;
				}

				return false;
			}

			USE_RESULT constexpr Size Find( const CharType item, Size offset ) const
			{
				return FirstIndexOf( item, offset );
			}

			USE_RESULT constexpr Size Find( Const_Ptr str, Size offset ) const
			{
				return FirstIndexOf( str, offset );
			}

			USE_RESULT constexpr Size rFind( const CharType item, Size offset ) const
			{
				return LastIndexOf( item, offset );
			}

			USE_RESULT constexpr Size rFind( Const_Ptr str, Size offset ) const
			{
				return LastIndexOf( str, offset );
			}


		private:
			constexpr void CheckWithinLength( const Size size, const char* msg ) const
			{
				if ( size > length )
					std::_Xout_of_range( msg );
			}

			Size GetStrLength( Const_Ptr str )
			{
				Size size = 0;
				auto currentChar = str;

				while ( *currentChar != null_terminator )
				{
					++size;
					++currentChar;
				}

				return size;
			}

		private:
			Const_Ptr string;
			size_t length;
		};

		// Type aliasing for standard forms of Template_String_View

#ifdef __cpp_lib_char8_t
		using u8StringView = Template_String_View<char8_t, std::char_traits<char8_t>>;

		USE_RESULT constexpr u8StringView operator"" _sv( u8StringView::Const_Ptr str, size_t length ) noexcept
		{
			return u8StringView( str, length );
		}
#endif // __cpp_lib_char8_t

		using StringView = Template_String_View<char, std::char_traits<char>>;
		using u16StringView = Template_String_View<char16_t, std::char_traits<char16_t>>;
		using u32StringView = Template_String_View<char32_t, std::char_traits<char32_t>>;
		using wStringView = Template_String_View<wchar_t, std::char_traits<wchar_t>>;


		USE_RESULT constexpr StringView operator"" _sv( StringView::Const_Ptr str, size_t length ) noexcept
		{
			return StringView( str, length );
		}

		USE_RESULT constexpr u16StringView operator"" _sv( u16StringView::Const_Ptr str, size_t length ) noexcept
		{
			return u16StringView( str, length );
		}

		USE_RESULT constexpr u32StringView operator"" _sv( u32StringView::Const_Ptr str, size_t length ) noexcept
		{
			return u32StringView( str, length );
		}

		USE_RESULT constexpr wStringView operator"" _sv( wStringView::Const_Ptr str, size_t length ) noexcept
		{
			return wStringView( str, length );
		}
	}

	template <typename T>
	class Iterator
	{
	public:
		using Ptr_Type = T*;
		using Ref_Type = T&;
		using Value_Type = T;

		Iterator( const Ref_Type obj )
			: ptr( std::addressof( obj ) )
		{}

		~Iterator()
		= default;

		Ptr_Type operator++( int )
		{
			const auto increment = ptr;
			++ptr;
			return increment;
		}

		Ptr_Type operator++()
		{
			return ++ptr;
		}

		Ptr_Type operator--( int )
		{
			const auto increment = ptr;
			--ptr;
			return increment;
		}

		Ptr_Type operator--()
		{
			return --ptr;
		}

		Ref_Type operator->()
		{
			return *ptr;
		}

		const Ref_Type operator->() const
		{
			return *ptr;
		}

	private:
		Ptr_Type ptr;
	};
}
