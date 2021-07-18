#include "pch.hpp"
#include "StringConverter_Test.hpp"

#include "../../Source/Utility/String/kStringConverter.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	StringConverterTester::StringConverterTester()
		: TesterBase("Character Converter Test")
	{}

	StringConverterTester::~StringConverterTester()
		= default;

	void StringConverterTester::Prepare() noexcept
	{
		ADD_TEST(ConvertUTF16ToUTF32());
		ADD_TEST(ConvertASCIIToUTF16());
		ADD_TEST(ConvertASCIIToUTF32());
		ADD_TEST(ConvertASCIIToUTF8());
	}
	
	void StringConverterTester::ConvertUTF16ToUTF32()
	{
		constexpr auto str16 = u"I'm a char32 ptr";
		const std::u32string str32 = klib::kString::Convert<char32_t>(str16);
		VERIFY(str32 == U"I'm a char32 ptr");
	}

	void StringConverterTester::ConvertASCIIToUTF16()
	{
		const std::string_view test = "test";
		const auto value = klib::kString::Convert<char16_t>(test);
		VERIFY(value == u"test");

		const auto str = std::string(test);
		const auto buf16str = klib::kString::Convert<char16_t>(str);
		VERIFY(buf16str == u"test");
	}
	
	void StringConverterTester::ConvertASCIIToUTF32()
	{
		const std::string_view test = "test";
		const auto buf32 = klib::kString::Convert<char32_t>(test);
		VERIFY(buf32 == U"test");
	}
	
	void StringConverterTester::ConvertASCIIToUTF8()
	{
		const std::string_view test = "unicode";
		const auto buf32 = klib::kString::Convert<char8_t>(test);
		VERIFY(buf32 == u8"unicode");

	}

}
#endif
