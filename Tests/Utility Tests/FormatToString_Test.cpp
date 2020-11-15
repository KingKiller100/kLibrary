#include "pch.hpp"
#include "FormatToString_Test.hpp"

#include "../../Source Files/Utility/String/kToString.hpp"

#ifdef TESTING_ENABLED


struct ObjectWithoutToString
{
	std::string str = "String made using identity overloading";
};

namespace klib::kString::stringify
{
	template<typename Char_t>
	class Identity<Char_t, ObjectWithoutToString>
	{
	public:
		constexpr Identity(const ObjectWithoutToString& obj)
			: data(Convert<Char_t>(obj.str))
		{}

		USE_RESULT constexpr decltype(auto) Get() const
		{
			return data.data();
		}

		USE_RESULT constexpr decltype(auto) GetPtr()
		{
			return std::addressof(data);
		}

	private:
		const StringWriter<Char_t>& data;
	};
}

namespace kTest::utility
{
	FormatToStringTester::FormatToStringTester()
		: Tester("Formatting To String Test")
	{	}

	FormatToStringTester::~FormatToStringTester()
		= default;

	void FormatToStringTester::Test()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(SprintfWrapperTest());
		VERIFY_MULTI(StringifyBinaryTest());
		VERIFY_MULTI(StringifyHexTest());
		VERIFY_MULTI(IdentityTest());
		VERIFY_MULTI(CustomTypeWithToStringTest());
		VERIFY_MULTI(CustomTypeWithoutToStringTest());
		VERIFY_MULTI(FormatToStringTest());
		VERIFY_MULTI(DirectToStringTest());
		VERIFY_MULTI_END();
	}

	using namespace klib;
	using namespace klib::kString;

	bool FormatToStringTester::IdentityTest()
	{
		{
			const auto input = std::u8string();
			const auto result = stringify::Identity<char8_t, decltype(input)>(input).Get();
			const auto expected = std::is_same_v<decltype(result), const char8_t* const>;
			VERIFY_COMPILE_TIME(expected);
		}
		
		{
			const auto input = "quarantine";
			const auto result = stringify::Identity<char, decltype(input)>(input).Get();
			const auto expected = std::is_same_v<decltype(result), const char* const>;
			VERIFY_COMPILE_TIME(expected);
		}
		
		{
			const auto input = "quarantine";
			const auto result = stringify::Identity<char, decltype(input)>(input).GetPtr();
			const auto expected = std::is_same_v<decltype(result), const char* const* const>;
			VERIFY_COMPILE_TIME(expected);
		}
		
		{
			const auto input = 5.6;
			const auto result = stringify::Identity<char, decltype(input)>(input).GetPtr();
			const auto expected = std::is_same_v<decltype(result), const double* const>;
			VERIFY_COMPILE_TIME(expected);
		}
		
		{
			const auto input = 5.6;
			const auto result = stringify::Identity<char, decltype(input)>(input).Get();
			const auto expected = std::is_same_v<decltype(result), const double>;
			VERIFY_COMPILE_TIME(expected);
		}
		
		return success;
	}


	bool FormatToStringTester::SprintfWrapperTest()
	{
		const struct ObjectWithToString
		{
			USE_RESULT const auto& ToString() const
			{
				return str;
			}

			const std::u32string str = U"klib::SprintfWrapper using object with ToString func";
		} example;

		{
			const auto input = std::string("STL strings can be handled by klib::SprintfWrapper");
			const auto result = stringify::SprintfWrapper("%s", input);
			constexpr auto expected = "STL strings can be handled by klib::SprintfWrapper";
			VERIFY(result == expected);
		}

		{
			const auto input = 100;
			const auto result = stringify::SprintfWrapper(L"%d", input);
			constexpr auto expected = L"100";
			VERIFY(result == expected);
		}

		{
			const auto result = stringify::SprintfWrapper(U"%s", example);
			constexpr auto expected = U"klib::SprintfWrapper using object with ToString func";
			VERIFY(result == expected);
		}

		return success;
	}

	bool FormatToStringTester::StringifyHexTest()
	{
		{
			constexpr auto input = 1000;
			const auto result = stringify::StringIntegralHex<char>(input, 8);
			constexpr auto expected = "000003e8";
			VERIFY(expected == result);
		}

		{
			constexpr auto input = -1000;
			const auto result = stringify::StringIntegralHex<char16_t>(input, 8);
			constexpr auto expected = u"fffffc18";
			VERIFY(expected == result);
		}

		{
			constexpr auto input = 4;
			const auto result = stringify::StringIntegralHex<char16_t>(input, 2);
			constexpr auto expected = u"04";
			VERIFY(expected == result);
		}

		{
			constexpr auto input = 24;
			const auto result = stringify::StringIntegralHex<char16_t>(input);
			constexpr auto expected = u"0000000000000018";
			VERIFY(expected == result);
		}

		return success;
	}

	bool FormatToStringTester::StringifyBinaryTest()
	{
		{
			constexpr auto input = 2ull;
			const auto result = stringify::StringIntegralBinary<char>(input, 4);
			VERIFY(result == "0010");
		}

		{ // Prints minimum digit of binary string, will fill remaining characters with '0' unless specified differently
			constexpr auto input = 64;
			const auto result = stringify::StringIntegralBinary<char16_t>(input, 8);
			VERIFY(result == u"01000000");
		}

		{ // Prints as many characters as necessary to represent the number, if min digits is less than the expected binary characters
			constexpr auto input = 1000;
			const auto result = stringify::StringIntegralBinary<char8_t>(input, 8);
			VERIFY(result == u8"1111101000");
		}

		{
			constexpr auto input = 4;
			const auto result = stringify::StringIntegralBinary<char32_t>(input, 0);
			VERIFY(result == U"100");
		}

		{ // if smaller than remaining characters, newly specified placeholder is used
			constexpr auto input = 4;
			const auto result = stringify::StringIntegralBinary<char16_t>(input, 5, 'a');
			VERIFY(result == u"aa100");
		}

		return success;
	}

	bool FormatToStringTester::CustomTypeWithToStringTest()
	{
		const struct ObjectWithToString
		{
			USE_RESULT const std::string& ToString() const
			{
				return str;
			}

			const std::string str = "Bitches ain't shit but hoes and tricks";
		} example;
		const auto result = ToString<char>(example);
		constexpr auto expected = "Bitches ain't shit but hoes and tricks";
		VERIFY(result == expected);

		return success;
	}

	bool FormatToStringTester::CustomTypeWithoutToStringTest()
	{
		ObjectWithoutToString owts;

		const auto result = ToString<char>(owts);
		constexpr auto expected = "String made using identity overloading";
		VERIFY(expected == result);

		return success;
	}

	bool FormatToStringTester::FormatToStringTest()
	{

		const auto tempIntPtr = std::make_unique<int>(76);

		const auto testStr = ToString("This test {0} ", 1U);
		const auto testStr2 = ToString("will all %s printf function format specifiers like with string literals ", "work");
		const auto testStr3W = ToString(L"and with different numerical types such as float {0:3}, ", 6.283f);
		const auto testStr4 = ToString("doubles {0:7}, ", 2.7182818);
		const auto testStr5 = ToString("unsigned ({1}) or signed integers ({0}), ", -50, 200U);
		const auto testStr6 = ToString("pointer addresses i.e. 0x{0} (random int ptr address)", tempIntPtr.get());
		const auto testStr9 = ToString("{0:b10}", 54321);
		const auto testStr10 = ToString("{0:h}", 54321);


		constexpr auto num = 1000;
		const auto hex = "0x" + stringify::StringIntegralHex<char>(num, 4, '0');
		const auto binary = "0b" + stringify::StringIntegralBinary<char>(num, 4, '0');

		VERIFY(testStr == "This test 1 ");
		VERIFY(testStr2 == "will all work printf function format specifiers like with string literals ");
		VERIFY(testStr3W == L"and with different numerical types such as float 6.283, ");
		VERIFY(testStr4 == "doubles 2.7182818, ");
		VERIFY(testStr5 == "signed (-50) or unsigned integers (200), ");
		VERIFY(testStr6.find("pointer addresses i.e. 0x00") != std::string::npos);
		VERIFY(testStr9 == "1101010000110001");
		VERIFY(testStr10 == "0000d431");
		VERIFY(hex == "0x03e8");
		VERIFY(binary == "1111101000");

		return success;
	}

	bool FormatToStringTester::DirectToStringTest()
	{
#ifdef __cpp_char8_t
		const auto test = klib::kString::ToString<char8_t>(980u, 123);
		VERIFY(test == u8"980123");
#else
		const auto test = klib::kString::ToString<char16_t>(980u);
		VERIFY(test == u"980");
#endif
		return success;
	}
}
#endif
