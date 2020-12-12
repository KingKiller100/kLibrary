#include "pch.hpp"
#include "SourceInfo_Test.hpp"

#include "../../Source Files/Utility/String/kStringConverter.hpp"
#include "../../Source Files/Utility/Debug/Source/kSourceInfo.hpp"

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
			VERIFY_COMPILE_TIME(source.line == 33);
			VERIFY_COMPILE_TIME(source.func == __FUNCTION__);
		}
		
		{
			constexpr auto source = SOURCE_INFO_NO_FUNC();
			VERIFY_COMPILE_TIME(source.file == __FILE__);
			VERIFY_COMPILE_TIME(source.line == 40);
			VERIFY_COMPILE_TIME(source.func == "");
		}
		
		{
			constexpr auto source = WSOURCE_INFO();
			VERIFY_COMPILE_TIME(source.file == __FILEW__);
			VERIFY_COMPILE_TIME(source.line == 47);
			VERIFY_COMPILE_TIME(source.func == __FUNCTIONW__);
		}
		
		{
			constexpr auto source = WSOURCE_INFO_NO_FUNC();
			VERIFY_COMPILE_TIME(source.file == __FILEW__);
			VERIFY_COMPILE_TIME(source.line == 54);
			VERIFY_COMPILE_TIME(source.func == L"");
		}

#ifdef MSVC_PLATFORM_TOOLSET
		{
			constexpr auto source = SOURCE_INFO_VS();
			VERIFY_COMPILE_TIME(source.file == __FILE__);
			VERIFY_COMPILE_TIME(source.line == 62);
			VERIFY_COMPILE_TIME(source.func == __FUNCSIG__);
		}
#endif

		{
			using Char_t = char16_t;
			constexpr auto  file = u"Test.txt";
			constexpr std::int32_t line = 212;
			constexpr auto func = u"void Test()";
			constexpr auto source = BasicSourceInfo<Char_t>(file, line, func);
			VERIFY_COMPILE_TIME(source.file == u"Test.txt");
			VERIFY_COMPILE_TIME(source.line == 212);
			VERIFY_COMPILE_TIME(source.func == u"void Test()");
		}
		
#ifdef __cpp_char8_t
		{
			using Char_t = char8_t;
			const std::basic_string<Char_t> file = Convert<Char_t>(__FILE__);
			const std::basic_string<Char_t> func = Convert<Char_t>(__FUNCTION__);
			const std::int32_t line = 73;
			const auto source = BasicSourceInfo<Char_t>(file, line, func);
			VERIFY(source.file == file);
			VERIFY(source.line == 73);
			VERIFY(source.func == func);
		}
#endif
		return success;
	}

	bool SourceInfoTester::InfoNoFuncTest()
	{
		return success;
	}

	bool SourceInfoTester::ToStringTest()
	{
		
		
		return success;
	}
}
#endif
