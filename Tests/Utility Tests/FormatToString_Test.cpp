#include "pch.hpp"
#include "FormatToString_Test.hpp"

#include "../../Source Files/Maths/Constants.hpp"
#include "../../Source Files/Utility/String/kToString.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	FormatToStringTester::FormatToStringTester()
		: Tester("Formatting To String Test")
	{	}

	FormatToStringTester::~FormatToStringTester()
		= default;

	void FormatToStringTester::Test()
	{
		VERIFY(FormatToStringTest() == true);
		VERIFY(SingleObjectToStringTest() == true);
	}

	struct Object
	{
		USE_RESULT const std::string& ToString() const
		{
			return str;
		}

		const std::string str = "Bitches ain't shit but hoes and tricks";
	};

	bool FormatToStringTester::FormatToStringTest()
	{
		using namespace klib::kString;

		Object o;
		const auto tempIntPtr = std::make_unique<int>(76);

		const auto testStr  = ToString("This test %d ", 1U);
		const auto testStr2 = ToString("will all %s printf function format specifiers like with string literals ", "work");
		const auto testStr3W = ToString(L"and with different numerical types such as float %.03f, ", static_cast<float>(kmaths::constants::TAU));
		const auto testStr4 = ToString("doubles {0:7}, ", kmaths::constants::E);
		const auto testStr5 = ToString("signed (%d) or unsigned integers (%u), ", -50, 200U);
		const auto testStr6 = ToString("pointer addresses i.e. 0x{0} (random int ptr address)", tempIntPtr.get());
		const auto testStr7 = ToString("%s", o);
		const auto testStr8 = stringify::SprintfWrapper("%s", std::string("STL strings can be handled by klib SprintfWrapper"));

		VERIFY(testStr == "This test 1 ");
		VERIFY(testStr2 == "will all work printf function format specifiers like with string literals ");
		VERIFY(testStr3W == L"and with different numerical types such as float 6.283, ");
		VERIFY(testStr4 == "doubles 2.7182818, ");
		VERIFY(testStr5 == "signed (-50) or unsigned integers (200), ");
		VERIFY(testStr6.find("pointer addresses i.e. 0x00") != std::string::npos);
		VERIFY(testStr7 == "Bitches ain't shit but hoes and tricks");
		VERIFY(testStr8 == "STL strings can be handled by klib SprintfWrapper");

		return success;
	}

	bool FormatToStringTester::SingleObjectToStringTest()
	{
#ifdef __cpp_char8_t
		const auto test = klib::kString::ToString<char8_t>(980u);
		VERIFY(test == u8"980");
#else
		const auto test = klib::kString::ToString<char16_t>(980u);
		VERIFY(test == u"980");
#endif
		return success;
	}
}
#endif
