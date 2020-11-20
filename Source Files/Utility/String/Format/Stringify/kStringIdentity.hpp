#pragma once

#include "../../kStringTypes.hpp"
#include "kStringExtract.hpp"

#include "../../../../HelperMacros.hpp"
#include "../../../../TypeTraits/StringTraits.hpp"
#include "../../../../TypeTraits/CustomTraits.hpp"
#include "../../../Debug/Exceptions/StringExceptions.hpp"

#include <type_traits>
#include <vector>


namespace klib::kString::stringify
{
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/// Primary template for unknown types - Crashes system if chosen
	/////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Char_t, typename T, typename Enabled_t = void>
	class Identity
	{
		static void CrashUnknownObject()
		{ // File cannot include kSprintf otherwise a circular dependency occurs
			constexpr auto format =
				"Type \"%s\" is not recognised/supported by " __FUNCTION__;

			const auto length = _snprintf(nullptr, 0, format
				, typeid(T).name());

			const auto buf = std::make_unique<char[]>(length + 1);

			std::sprintf(buf.get(), format
				, typeid(T).name());

			const auto msg = std::string(buf.get(), buf.get() + length - 1);

			throw kDebug::FormatError(msg);
		}

	public:
		constexpr Identity() = delete;

		USE_RESULT static decltype(auto) Get(const T& val) noexcept
		{
			CrashUnknownObject();
		}

		USE_RESULT static decltype(auto) MakeStr(const T& arg, StringWriter<Char_t>& specifier)
		{
			CrashUnknownObject();
		}
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/// User made Custom types - Must have a function ToString that returns an object with a 
	/// "data()" function which returns a const CharType pointer
	/////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Char_t, typename T>
	const StringWriter<Char_t>& GetObjectString(const T& obj)
	{
		static std::vector<StringWriter<Char_t>> storage =
			decltype(storage)();

		const auto temp = Convert<Char_t>(obj.ToString());

		auto iter = std::find(storage.begin(), storage.end(), temp);
		if (iter != storage.end())
			return (*iter);

		const auto& value = storage.emplace_back(temp);
		return value;
	}

	template<typename Char_t, typename T>
	class Identity<Char_t, T, std::enable_if_t<type_trait::Is_CustomType_V<T>>>
	{
	public:
		USE_RESULT static decltype(auto) Get(const T& val) noexcept
		{
			const std::basic_string<Char_t>& string = GetObjectString<Char_t>(val);
			return string.data();
		}

		USE_RESULT static decltype(auto) MakeStr(const T& arg, UNUSED StringWriter<Char_t>& specifier)
		{
			const std::basic_string<Char_t>& string = GetObjectString<Char_t>(arg);
			return string.data();
		}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	/// C++ STL string/string_view
	///////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Char_t, typename T>
	class Identity<Char_t, T, std::enable_if_t<
		type_trait::Is_CharType_V<Char_t>
		&& type_trait::Is_StringType_V<T>
		&& std::is_same_v<typename T::value_type, Char_t>
		>>
	{
	public:
		USE_RESULT static decltype(auto) Get(const T& str) noexcept
		{
			return str.data();
		}

		USE_RESULT static const typename T::value_type* MakeStr(const T& arg, StringWriter<Char_t>& specifier)
		{
			const auto& str = stringify::HandleSTLString(arg, specifier);
			return str.data();
		}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	/// Character Pointers
	///////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Char_t, typename T>
	class Identity<Char_t, T, std::enable_if_t<
		type_trait::Is_CharType_V<Char_t>
		&& type_trait::Is_CString_V<T>
		&& std::is_same_v<Char_t,
		std::remove_cv_t<
		std::remove_pointer_t<
		std::decay_t<T>>>>
		>>
	{
	public:
		USE_RESULT static decltype(auto) Get(const T val) noexcept
		{
			return val;
		}

		USE_RESULT static decltype(auto) MakeStr(const T arg, StringWriter<Char_t>& specifier)
		{
			return stringify::HandleCharPointer(arg, specifier);
		}
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////
	///// Any Non-Character Pointers
	///////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Char_t, typename T>
	class Identity<Char_t, T, std::enable_if_t<
		type_trait::Is_CharType_V<Char_t>
		&& !type_trait::Is_CharType_V<ONLY_TYPE(T)>
		&& std::is_pointer_v<T>
		>>
	{
	public:
		USE_RESULT static decltype(auto) Get(T val) noexcept
		{
			return val;
		}

		USE_RESULT static decltype(auto) MakeStr(T arg, StringWriter<Char_t>& specifier)
		{
			return stringify::HandlePointer<Char_t>(arg, specifier);
		}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///// Primitive types (char, int, double, unsigned long long,...)
	///////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Char_t, typename T>
	class Identity<Char_t, T, std::enable_if_t<
		type_trait::Is_CharType_V<Char_t>
		&& std::is_arithmetic_v<T>
		>>
	{
	public:
		USE_RESULT static decltype(auto) Get(T val) noexcept
		{
			return val;
		}

		USE_RESULT static decltype(auto) MakeStr(T value, StringWriter<Char_t>& specifier)
		{
			if constexpr (std::is_floating_point_v<T>)
				return  stringify::HandleFloat<Char_t>(value, specifier);
			else if constexpr (std::is_same_v<ONLY_TYPE(T), bool>)
				return stringify::HandleBool<Char_t>(value, specifier);
			else
				return stringify::HandleInteger<Char_t>(value, specifier);
		}

	private:
		const T data;
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////
	///// Primitive array types (char, int, double, unsigned long long,...)
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//template<typename CharType, typename T, size_t Size>
	//constexpr
	//	std::enable_if_t <
	//	type_trait::Is_CharType_V<CharType>
	//	&& std::is_arithmetic_v<T>
	//	&& !std::is_pointer_v<T>
	//	&& std::is_array_v<T[Size]>
	//	, const T*>
	//	Identity(const T(&obj)[Size])
	//{
	//	return std::addressof(obj[0]);
	//}

	//template<typename CharType, typename T, size_t Size>
	//constexpr
	//	std::enable_if_t <
	//	type_trait::Is_CharType_V<CharType>
	//	&& std::is_arithmetic_v<T>
	//	&& !std::is_pointer_v<T>
	//	&& std::is_array_v<T[]>
	//	, const T* const*>
	//	IdentityPtr(const T(&obj)[Size])
	//{
	//	auto ptr = Identity<CharType, T, Size>(obj);
	//	return std::addressof(ptr);
	//}

}
