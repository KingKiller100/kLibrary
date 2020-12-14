#include "pch.hpp"
#include "SourceInfo_Test.hpp"

#include "../../Source Files/Utility/String/kToString.hpp"
#include "../../Source Files/Utility/String/kStringConverter.hpp"
#include "../../Source Files/Utility/Debug/Source/kSourceInfo.hpp"
#include "../../Source Files/Utility/Debug/Source/kSourceInfoToString.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	SourceInfoTester::SourceInfoTester()
		: TesterBase("Source Code Information Test")
	{}

	SourceInfoTester::~SourceInfoTester()
	{
	}

	void SourceInfoTester::Test()
	{
		VERIFY_MULTI_INIT()
		VERIFY_MULTI(InfoTest())
		VERIFY_MULTI(ToStringTest())
		VERIFY_MULTI_END()
	}
	
	using namespace klib;
	using namespace kDebug;
	using namespace kString;
	
	bool SourceInfoTester::InfoTest()
	{
		{
			constexpr auto source = SOURCE_INFO();
			VERIFY_COMPILE_TIME(source.file == __FILE__);
			VERIFY_COMPILE_TIME(source.line == 35);
			VERIFY_COMPILE_TIME(source.func == __FUNCTION__);
		}
		
		{
			constexpr auto source = ::klib::kDebug::wSourceInfo(WIDE_STR(__FILE__), kSOURCEINFOIMPL_FILELINE, WIDE_STR(__FUNCTION__), WIDE_STR(__TIMESTAMP__));
			VERIFY_COMPILE_TIME(source.file == __FILEW__);
			VERIFY_COMPILE_TIME(source.line == 42);
			VERIFY_COMPILE_TIME(source.func == __FUNCTIONW__);
		}
		
		{
			const auto source = U16SOURCE_INFO();
			VERIFY_COMPILE_TIME(source.file == U16_STR( __FILE__ ));
			VERIFY_COMPILE_TIME(source.line == 49);
			VERIFY_COMPILE_TIME(source.func == U16_STR( __FUNCTION__ ));
		}
		
		{
			const auto source = U32SOURCE_INFO();
			VERIFY_COMPILE_TIME(source.file == U32_STR( __FILE__ ));
			VERIFY_COMPILE_TIME(source.line == 56);
			VERIFY_COMPILE_TIME(source.func == U32_STR( __FUNCTION__ ));
		}

#ifdef MSVC_PLATFORM_TOOLSET
		{
			constexpr auto source = SOURCE_INFO_VS();
			VERIFY_COMPILE_TIME(source.file == __FILE__);
			VERIFY_COMPILE_TIME(source.line == 64);
			VERIFY_COMPILE_TIME(source.func == __FUNCSIG__);
		}
		
		{
			constexpr auto source = U16SOURCE_INFO_VS();
			VERIFY_COMPILE_TIME(source.file == U16_STR(__FILE__));
			VERIFY_COMPILE_TIME(source.line == 71);
			VERIFY_COMPILE_TIME(source.func == U16_STR(__FUNCSIG__));
		}
#endif

		{
			using Char_t = char16_t;
			constexpr auto  file = u"Test.txt";
			constexpr std::int32_t line = 212;
			constexpr auto func = u"void Test()";
			constexpr auto timeStamp = u"Mon Dec 14 01:03:52 2020";
			const auto source = BasicSourceInfo<Char_t>(file, line, func, timeStamp);
			VERIFY_COMPILE_TIME(source.file == u"Test.txt");
			VERIFY_COMPILE_TIME(source.line == 212);
			VERIFY_COMPILE_TIME(source.func == u"void Test()");
			VERIFY_COMPILE_TIME(source.timeStamp == u"Mon Dec 14 01:03:52 2020");
		}
		
#ifdef __cpp_char8_t
		{
			using Char_t = char8_t;
			const std::basic_string<Char_t> file = Convert<Char_t>(__FILE__);
			const std::basic_string<Char_t> func = Convert<Char_t>(__FUNCTION__);
			const std::int32_t line = 73;
			constexpr auto timeStamp = "Mon Dec 14 01:03:52 2020";
			const auto source = BasicSourceInfo<Char_t>(file, line, func, "");
			VERIFY(source.file == file);
			VERIFY(source.line == 73);
			VERIFY(source.func == func);
		}
#endif
		
		return success;
	}

	bool SourceInfoTester::InfoNoFuncTest()
	{
		{
			constexpr auto source = SOURCE_INFO_NO_FUNC();
			VERIFY_COMPILE_TIME(source.file == __FILE__);
			VERIFY_COMPILE_TIME(source.line == 99);
			VERIFY_COMPILE_TIME(source.func == "");
		}

		{
			constexpr auto source = WSOURCE_INFO_NO_FUNC();
			VERIFY_COMPILE_TIME(source.file == __FILEW__);
			VERIFY_COMPILE_TIME(source.line == 106);
			VERIFY_COMPILE_TIME(source.func == L"");
		}
		
		return success;
	}

	bool SourceInfoTester::ToStringTest()
	{
		{
			constexpr auto source = SOURCE_INFO();
			const auto result = kString::ToString("{0:l}", source);
			
		}
		
		return success;
	}
}
#endif
