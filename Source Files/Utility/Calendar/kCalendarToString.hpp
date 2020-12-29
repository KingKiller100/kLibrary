#pragma once

#include "Date/kDate.hpp"
#include "Time/kTime.hpp"
#include "../String/kToString.hpp"

namespace klib::kString::stringify
{
	using namespace klib::kCalendar;

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Date Components //////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename Char_t>
	class Identity<Char_t, Day>
	{
	public:
		using Type = Day;

		USE_RESULT static decltype(auto) MakeStr(const Type& hour, StringWriter<Char_t>& specifier)
		{
			const auto count = TryStrTo(specifier, 1);
			const auto format = std::string(count, Type::FormatToken);
			const auto str = hour.ToString(format);

			return Convert<Char_t>(str);
		}
	};

	template<typename Char_t>
	class Identity<Char_t, Month>
	{
	public:
		using Type = Month;

		USE_RESULT static decltype(auto) MakeStr(const Type& hour, StringWriter<Char_t>& specifier)
		{
			const auto count = TryStrTo(specifier, 1);
			const auto format = std::string(count, Type::FormatToken);
			const auto str = hour.ToString(format);

			return Convert<Char_t>(str);
		}
	};

	template<typename Char_t>
	class Identity<Char_t, Year>
	{
	public:
		using Type = Year;

		USE_RESULT static decltype(auto) MakeStr(const Type& hour, StringWriter<Char_t>& specifier)
		{
			const auto count = TryStrTo(specifier, 1);
			const auto format = std::string(count, Type::FormatToken);
			const auto str = hour.ToString(format);

			return Convert<Char_t>(str);
		}
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Time Components //////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename Char_t>
	class Identity<Char_t, Hour>
	{
	public:
		using Type = Hour;
		
		USE_RESULT static decltype(auto) MakeStr(const Type& hour, StringWriter<Char_t>& specifier)
		{
			const auto count = TryStrTo(specifier, 1);
			const auto format = std::string(count, Type::FormatToken);
			const auto str = hour.ToString(format);

			return Convert<Char_t>(str);
		}
	};
	
	template<typename Char_t>
	class Identity<Char_t, Minute>
	{
	public:
		using Type = Minute;
		
		USE_RESULT static decltype(auto) MakeStr(const Type& minute, StringWriter<Char_t>& specifier)
		{
			const auto count = TryStrTo(specifier, 1);
			const auto format = std::string(count, Type::FormatToken);
			const auto str = minute.ToString(format);

			return Convert<Char_t>(str);
		}
	};
	
	template<typename Char_t>
	class Identity<Char_t, Second>
	{
	public:
		using Type = Second;
		
		USE_RESULT static decltype(auto) MakeStr(const Type& second, StringWriter<Char_t>& specifier)
		{
			const auto count = TryStrTo(specifier, 1);
			const auto format = std::string(count, Type::FormatToken);
			const auto str = second.ToString(format);

			return Convert<Char_t>(str);
		}
	};
	
	template<typename Char_t>
	class Identity<Char_t, Millisecond>
	{
	public:
		using Type = Millisecond;
		
		USE_RESULT static decltype(auto) MakeStr(const Type& milli, StringWriter<Char_t>& specifier)
		{
			const auto count = TryStrTo(specifier, 1);
			const auto format = std::string(count, Type::FormatToken);
			const auto str = milli.ToString(format);

			return Convert<Char_t>(str);
		}
	};

}
