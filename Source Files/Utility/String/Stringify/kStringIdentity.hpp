#pragma once

#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"
#include "../../../TypeTraits/CustomTraits.hpp"

#include "../kStringTypes.hpp"

#include <type_traits>
#include <vector>

#include "../Type/kStringExtract.hpp"

namespace klib::kString::stringify
{
	template<typename Char_t, typename T, typename Enabled_t = void>
	class Identity
	{
		constexpr Identity() = delete;

		USE_RESULT static decltype(auto) MakeStr(const T& arg, StringWriter<Char_t>& specifier)
		{
			const auto msg = stringify::SprintfWrapper(
				"Type \"%s\" is not recognised/supported by " __FUNCTION__
				, typeid(T).name());
			throw kDebug::FormatError(msg);
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
		constexpr Identity(const T& obj)
			: data(obj)
		{}

		USE_RESULT constexpr decltype(auto) Get() const
		{
			const std::basic_string<Char_t>& string = GetObjectString<Char_t>(data);
			return string.data();
		}

		USE_RESULT constexpr decltype(auto) GetPtr() const
		{
			const std::basic_string<Char_t>& string = GetObjectString<Char_t>(data);
			return std::addressof(string);
		}

		USE_RESULT static constexpr decltype(auto) Type()
		{
			return typeid(T).name();
		}

		USE_RESULT static decltype(auto) MakeStr(const T& arg, StringWriter<Char_t>& specifier)
		{
			const std::basic_string<Char_t>& string = GetObjectString<Char_t>(arg);
			return string.data();
		}

	private:
		const T& data;
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
		constexpr Identity(const T& str)
			: data(str)
		{}

		USE_RESULT constexpr auto Get() const
		{
			return data.data();
		}

		USE_RESULT constexpr auto GetPtr() const
		{
			return std::addressof(data);
		}

		USE_RESULT static decltype(auto) MakeStr(const T& arg, StringWriter<Char_t>& specifier)
		{
			return impl::HandleStringAndInsertInOutput(arg, specifier);
		}

	private:
		const T& data;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	/// Character Pointers
	///////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Char_t, typename T>
	class Identity<Char_t, T, std::enable_if_t<
		type_trait::Is_CharType_V<Char_t>
		&& type_trait::Is_CharType_V<ONLY_TYPE(T)>
		&& std::is_same_v<Char_t, ONLY_TYPE(T)>
		&& std::is_pointer_v<T>
		>>
	{
	public:
		constexpr Identity(const T& str)
			: data(str)
		{}

		USE_RESULT constexpr auto Get() const
		{
			return data;
		}

		USE_RESULT constexpr auto GetPtr() const
		{
			return std::addressof(data);
		}

		USE_RESULT static decltype(auto) MakeStr(const T arg, StringWriter<Char_t>& specifier)
		{
			return impl::HandleCharPointer(arg, specifier);
		}

	private:
		const T& data;
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
		constexpr Identity(const T& str)
			: data(str)
		{}

		USE_RESULT constexpr auto Get() const
		{
			return data;
		}

		USE_RESULT constexpr auto GetPtr() const
		{
			return reinterpret_cast<const void* const*>(std::addressof(data));
		}

		USE_RESULT static decltype(auto) MakeStr(T arg, StringWriter<Char_t>& specifier)
		{
			return impl::HandlePointer<Char_t>((void*)arg, specifier);
		}

	private:
		const T& data;
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
		constexpr Identity(const T& str)
			: data(str)
		{}

		USE_RESULT constexpr decltype(auto) Get() const
		{
			return data;
		}

		USE_RESULT constexpr decltype(auto) GetPtr() const
		{
			return std::addressof(data);
		}

		USE_RESULT static decltype(auto) MakeStr(T value, StringWriter<Char_t>& specifier)
		{
			if constexpr (std::is_floating_point_v<T>)
				return  impl::HandleFloat<Char_t>(value, specifier);
			else if constexpr (std::is_same_v<ONLY_TYPE(T), bool>)
				return impl::HandleBool<Char_t>(value, specifier);
			else
				return impl::HandleInteger<Char_t>(value, specifier);
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
