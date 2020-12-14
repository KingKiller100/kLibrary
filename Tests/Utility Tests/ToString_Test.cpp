#include "pch.hpp"
#include "ToString_Test.hpp"

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
		USE_RESULT static decltype(auto) Get(const ObjectWithoutToString& val) noexcept
		{
			return val.str.data();
		}

		USE_RESULT static decltype(auto) MakeStr(const ObjectWithoutToString& obj, StringWriter<Char_t>& specifier)
		{
			return obj.str;
		}
	};
}

namespace kTest::utility
{
	FormatToStringTester::FormatToStringTester()
		: TesterBase("Formatting To String Test")
	{	}

	FormatToStringTester::~FormatToStringTester()
		= default;

	void FormatToStringTester::Test()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(SprintfWrapperTest());
		VERIFY_MULTI(StringifyFloatingPointTest());
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
#ifdef __cpp_char8_t
		{
			const auto input = std::u8string();
			const auto result = stringify::Identity<char8_t, decltype(input)>::Get(input);
			const auto expected = std::is_same_v<decltype(result), const char8_t* const>;
			VERIFY_COMPILE_TIME(expected);
		}
#endif

		{
			const auto input = "quarantine";
			constexpr auto g = type_trait::Is_CharType_V<char>;
			constexpr auto h = type_trait::Is_CString_V<decltype(input)>;
			constexpr auto i = std::is_same_v<char, std::remove_cv_t<std::remove_pointer_t<
				std::decay_t<decltype(input)>>>>;
			const auto result = stringify::Identity<char, const char[]>::Get(input);
			const auto expected = std::is_same_v<decltype(result), const char* const>;
			VERIFY_COMPILE_TIME(expected);
		}

		{
			const auto input = std::string_view("quarantine");
			const auto result = stringify::Identity<char, decltype(input)>::Get(input);
			const auto expected = std::is_same_v<decltype(result), const char* const>;
			VERIFY_COMPILE_TIME(expected);
		}

		{
			const auto input = 5.6;
			const auto result = stringify::Identity<char, decltype(input)>::Get(input);
			const auto expected = std::is_same_v<decltype(result), const double>;
			VERIFY_COMPILE_TIME(expected);
		}

		{
			const auto input = 5.6f;
			const auto result = stringify::Identity<char, decltype(input)>::Get(input);
			const auto expected = std::is_same_v<decltype(result), const float>;
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

			const std::u32string str = U"klib::Sprintf using object with ToString func";
		} example;

		{
			const auto input = std::string("STL strings can be handled by klib::Sprintf");
			const auto result = Sprintf("%s", input);
			constexpr auto expected = "STL strings can be handled by klib::Sprintf";
			VERIFY(result == expected);
		}

		{
			const auto input = 100;
			const auto result = Sprintf(L"%d", input);
			constexpr auto expected = L"100";
			VERIFY(result == expected);
		}

		{
			const auto format = ToWriter(U"%s");
			const auto result = Sprintf(format, example);
			constexpr auto expected = U"klib::Sprintf using object with ToString func";
			VERIFY(result == expected);
		}

		return success;
	}

	bool FormatToStringTester::StringifyFloatingPointTest()
	{
		{
			const auto num = std::numeric_limits<float>::quiet_NaN();
			const std::string result = stringify::StringFloatingPoint<char>(num);
			VERIFY(result == "nan");
		}

		{
			const auto num = std::numeric_limits<double>::signaling_NaN();
			const std::string result = stringify::StringFloatingPoint<char>(num);
			VERIFY(result == "nan");
		}

		{
			const auto num = std::numeric_limits<long double>::infinity();
			const std::string result = stringify::StringFloatingPoint<char>(num);
			VERIFY(result == "inf");
		}

		{
			constexpr auto num = 2.5;
			const std::string result = stringify::StringFloatingPoint<char>(num, 1, stringify::FloatFormat::FIX);
			VERIFY(result == "2.5");
		}

		{
			constexpr auto num = -2.0075;
			const std::u16string result = stringify::StringFloatingPoint<char16_t>(num, 4);
			VERIFY(result == u"-2.0075")
		}

		{
			constexpr auto num = -200.125;
			const std::u16string result = stringify::StringFloatingPoint<char16_t>(num, 2);
			VERIFY(result == u"-200.13")
		}

		{
			constexpr auto num = 50.75;
			const std::u16string result = stringify::StringFloatingPoint<char16_t>(num, 0);
			VERIFY(result == u"51")
		}

#if __cpp_char8_t
		{
			constexpr auto num = 2.5f;
			const std::u8string result = stringify::StringFloatingPoint<char8_t>(num, 3);
			VERIFY(result == u8"2.500")
		}

		{
			constexpr auto num = 2.5e0;
			const std::u8string result = stringify::StringFloatingPoint<char8_t>(num, 1, stringify::FloatFormat::SCI);
			VERIFY(result == u8"2.5e+00")
		}
#endif

		{
			constexpr auto num = 2.5e3;
			const std::u32string result = stringify::StringFloatingPoint<char32_t>(num, 3, stringify::FloatFormat::SCI);
			VERIFY(result == U"2.500e+03")
		}

		{
			constexpr auto num = 2e5;
			const std::u32string result = stringify::StringFloatingPoint<char32_t>(num, 4, stringify::FloatFormat::SCI);
			const auto expected = Sprintf("%.4e", num);
			VERIFY(result == U"2.0000e+05")
		}

		{
			constexpr auto num = 5e-1f;
			const std::string result = stringify::StringFloatingPoint<char>(num, 0, stringify::FloatFormat::SCI);
			const auto expected = Sprintf("%.0e", num);
			VERIFY(result == "5e-01")
		}

		{
			constexpr auto num = 6.25e-2;
			const std::string result = stringify::StringFloatingPoint<char>(num, 3, stringify::FloatFormat::SCI);
			const auto expected = Sprintf("%.3e", num);
			VERIFY(result == "6.250e-02")
		}

		{
			constexpr auto num = 6.25e-5;
			const std::string result = stringify::StringFloatingPoint<char>(num, 2, stringify::FloatFormat::SCI);
			const auto expected = Sprintf("%.2e", num);
			VERIFY(result == "6.25e-05")
		}

		{
			constexpr auto num = 2 + 6.25549769348e-5;
			const std::string result = stringify::StringFloatingPoint<char>(num, 19, stringify::FloatFormat::SCI);
			const auto expected = Sprintf("%.19e", num);
			VERIFY(result == "2.0000625549769348588e+00")
		}

		return success;
	}

	bool FormatToStringTester::StringifyHexTest()
	{
		{
			constexpr auto input = 1000;
			const auto result = ToWriter(stringify::StringIntegralHex<char>(input, 8));
			constexpr auto expected = "000003e8";
			VERIFY(expected == result);
		}

		{
			constexpr auto input = -1000;
			const auto result = ToWriter(stringify::StringIntegralHex<char16_t>(input, 8));
			constexpr auto expected = u"fffffc18";
			VERIFY(expected == result);
		}

		{
			constexpr auto input = 4;
			const auto result = ToWriter(stringify::StringIntegralHex<char16_t>(input, 2));
			constexpr auto expected = u"04";
			VERIFY(expected == result);
		}

		{
			constexpr auto input = 24;
			const auto result = ToWriter(stringify::StringIntegralHex<char16_t>(input));
			constexpr auto expected = u"0000000000000018";
			VERIFY(expected == result);
		}

		{
			constexpr auto input = 0xff100;
			const auto result = ToWriter(stringify::StringIntegralHex<char16_t>(input, 3));
			constexpr auto expected = u"f100";
			VERIFY(expected == result);
		}

		return success;
	}

	bool FormatToStringTester::StringifyBinaryTest()
	{
		{
			constexpr auto input = 2ull;
			const auto result = ToWriter(stringify::StringIntegralBinary<char>(input, 4));
			VERIFY(result == "0010");
		}

		// Prints minimum digit of binary string, will fill remaining characters with '0' unless specified differently
		{
			constexpr auto input = 64;
			const auto result = ToWriter(stringify::StringIntegralBinary<char16_t>(input, 8));
			VERIFY(result == u"01000000");
		}

#ifdef __cpp_char8_t
		// Prints as many characters as necessary to represent the number, if min digits is less than the expected binary characters
		{
			constexpr auto input = 1000;
			const auto result = ToWriter(stringify::StringIntegralBinary<char8_t>(input, 8));
			VERIFY(result == u8"1111101000");
		}
#endif

		{
			constexpr auto input = 4;
			const auto result = ToWriter(stringify::StringIntegralBinary<char32_t>(input, 0));
			VERIFY(result == U"100");
		}

		// if smaller than remaining characters, newly specified placeholder is used
		{
			constexpr auto input = 4;
			const auto result = ToWriter(stringify::StringIntegralBinary<char16_t>(input, 5, 'a'));
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
		const auto testStr2 = ToString("will {0} with all string literals ", "work");
		const auto testStr3W = ToString(L"and with different numerical types such as float {0:3}, ", 6.283f);
		const auto testStr4 = ToString("doubles {0:7}, ", 2.7182818);
		const auto testStr5 = ToString("unsigned ({1}) or signed integers ({0}), ", -50, 200U);
		const auto testStr6 = ToString("pointer addresses i.e. 0x{0} (random int ptr address)", tempIntPtr.get());
		const auto testStr7 = ToString("Repeat the same value in different ways. num:{0} hex:{0:h8} bin:{0:b}", 54321);
		const auto testStr8 = ToString("Booleans - Text:{0} Int:{0:d}", true);

		constexpr auto num = 1000;
		const auto hex = "0x" + std::string(stringify::StringIntegralHex<char>(num, 4, '0'));

#ifdef __cpp_char8_t
		const auto binary = u8"0b" + std::u8string(stringify::StringIntegralBinary<char8_t>(num, 4, '0'));
		VERIFY(binary == u8"0b1111101000");
#endif

		VERIFY(testStr == "This test 1 ");
		VERIFY(testStr2 == "will work with all string literals ");
		VERIFY(testStr3W == L"and with different numerical types such as float 6.283, ");
		VERIFY(testStr4 == "doubles 2.7182818, ");
		VERIFY(testStr5 == "unsigned (200) or signed integers (-50), ");
		VERIFY(testStr6.find("pointer addresses i.e. 0x00") != std::string::npos);
		VERIFY(testStr7 == "Repeat the same value in different ways. num:54321 hex:0000d431 bin:1101010000110001");
		VERIFY(testStr8 == "Booleans - Text:true Int:1");
		VERIFY(hex == "0x03e8");

		return success;
	}

	bool FormatToStringTester::DirectToStringTest()
	{
		{
#ifdef __cpp_char8_t
			const auto test = klib::kString::ToString<char8_t>(980u, 123);
			VERIFY(test == u8"980123");
#else
			const auto test = klib::kString::ToString<char16_t>(980u);
			VERIFY(test == u"980");
#endif
		}

		{
			const auto result = ToString<char>(1, 2, 3, 4, "five", 6, 7, 8, 9, "ten", 11, 12, 13, 14, "fifteen");
			const char expected[] = "1234five6789ten11121314fifteen";
			VERIFY(result == expected);
		}

		return success;
	}
}
#endif
