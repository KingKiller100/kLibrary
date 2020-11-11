#include "pch.hpp"
#include "StringConverter_Test.hpp"

#include "../../Source Files/Utility/String/kStringConverter.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	UTFConverterTester::UTFConverterTester()
		: Tester("UTF Converter Test")
	{}

	UTFConverterTester::~UTFConverterTester()
		= default;

	void UTFConverterTester::Test()
	{
		constexpr auto str16 = u"I'm a char32 ptr";
		const std::u32string str32 = klib::kString::Convert<char32_t>(str16);
		VERIFY(str32 == U"I'm a char32 ptr");
		
		const std::string_view test = "test";
		const auto value = klib::kString::Convert<char16_t>(test);
		VERIFY(value == u"test");

		const auto buf32 = klib::kString::Convert<char32_t>(test);
		VERIFY(buf32 == U"test");
		
		const auto str = std::string(test);
		const auto buf16str = klib::kString::Convert<char16_t>(str);
		VERIFY(buf16str == u"test");
	}
}
#endif
