#include "pch.hpp"
#include "SourceInfo_Test.hpp"

#include "../../Source Files/Utility/String/kToString.hpp"
#include "../../Source Files/Utility/String/kStringConverter.hpp"
#include "../../Source Files/Utility/Debug/Source/kSourceInfo.hpp"
#include "../../Source Files/Utility/Debug/Source/kMutableSourceInfo.hpp"
#include "../../Source Files/Utility/Debug/Source/kSourceInfoToString.hpp"
#include "../../Source Files/Utility/Debug/Source/kMutableSourceInfoToString.hpp"

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
		VERIFY_MULTI(InfoNoFuncTest())
		VERIFY_MULTI(ToStringTest())
		VERIFY_MULTI(MutInfoTest())
		VERIFY_MULTI(MutInfoNoFuncTest())
		VERIFY_MULTI(MutToStringTest())
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
			VERIFY_COMPILE_TIME(source.line == kFILELINE - 2);
			VERIFY_COMPILE_TIME(source.func == __FUNCTION__);
		}
		
		{
			constexpr auto source = WSOURCE_INFO();
			VERIFY_COMPILE_TIME(source.file == __FILEW__);
			VERIFY_COMPILE_TIME(source.line == kFILELINE - 2);
			VERIFY_COMPILE_TIME(source.func == __FUNCTIONW__);
		}
		
		{
			const auto source = U16SOURCE_INFO();
			VERIFY_COMPILE_TIME(source.file == U16_STR( __FILE__ ));
			VERIFY_COMPILE_TIME(source.line == kFILELINE - 2);
			VERIFY_COMPILE_TIME(source.func == U16_STR( __FUNCTION__ ));
		}
		
		{
			const auto source = U32SOURCE_INFO();
			VERIFY_COMPILE_TIME(source.file == U32_STR( __FILE__ ));
			VERIFY_COMPILE_TIME(source.line == kFILELINE - 2);
			VERIFY_COMPILE_TIME(source.func == U32_STR( __FUNCTION__ ));
		}

#ifdef MSVC_PLATFORM_TOOLSET
		{
			constexpr auto source = SOURCE_INFO_VS();
			VERIFY_COMPILE_TIME(source.file == __FILE__);
			VERIFY_COMPILE_TIME(source.line == kFILELINE - 2);
			VERIFY_COMPILE_TIME(source.func == __FUNCSIG__);
		}
		
		{
			constexpr auto source = U16SOURCE_INFO_VS();
			VERIFY_COMPILE_TIME(source.file == U16_STR(__FILE__));
			VERIFY_COMPILE_TIME(source.line == kFILELINE - 2);
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
		
		{
			using Char_t = char32_t;
			const std::basic_string<Char_t> file = Convert<Char_t>(__FILE__);
			const std::basic_string<Char_t> func = Convert<Char_t>(__FUNCTION__);
			const std::int32_t line = 73;
			constexpr auto timeStamp = "Mon Dec 14 01:03:52 2020";
			const auto source = BasicSourceInfo<Char_t>(file, line, func, U"");
			VERIFY(source.file == file);
			VERIFY(source.line == 73);
			VERIFY(source.func == func);
		}
		
#ifdef __cpp_char8_t
		{
			using Char_t = char8_t;
			const std::basic_string<Char_t> file = Convert<Char_t>(__FILE__);
			const std::basic_string<Char_t> func = Convert<Char_t>(__FUNCTION__);
			const std::int32_t line = 73;
			constexpr auto timeStamp = "Mon Dec 14 01:03:52 2020";
			const auto source = BasicSourceInfo<Char_t>(file, line, func, u8"");
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
			const auto source = SOURCE_INFO_NO_FUNC();
			VERIFY_COMPILE_TIME(source.file == __FILE__);
			VERIFY_COMPILE_TIME(source.line == kFILELINE - 2);
			VERIFY_COMPILE_TIME(source.func == "");
		}

		{
			constexpr auto source = WSOURCE_INFO_NO_FUNC();
			VERIFY_COMPILE_TIME(source.file == __FILEW__);
			VERIFY_COMPILE_TIME(source.line == kFILELINE - 2);
			VERIFY_COMPILE_TIME(source.func == L"");
		}
		
		return success;
	}

	bool SourceInfoTester::ToStringTest()
	{
		std::string file = __FILE__;
		std::string func = __FUNCTION__;
		
		{
			const auto source = SOURCE_INFO();
			const auto result = ToString("{0:l}", source);
			VERIFY(result == "Line: 151");
		}
		
		{
			const auto source = SOURCE_INFO();
			const auto result = ToString("{0:f}", source);
			VERIFY(result == "File: " + file);
		}
		
		{
			const auto source = SOURCE_INFO();
			const auto result = ToString("{0:fl}", source);
			VERIFY(result == "File: " + file + " [163]");
		}
		
		{
			const auto source = SOURCE_INFO();
			const auto result = ToString("{0:z}", source);
			VERIFY(result == "Function: " + func);
		}
		
		{
			const auto source = SOURCE_INFO();
			const auto result = ToString("{0:t}", source);
			VERIFY(result.find("Time Stamp: ") != std::string::npos);
		}
		
		{
			const auto source = SOURCE_INFO();
			const auto result = ToString("{0:a}", source);
			VERIFY(result.find("File: " + file + " Line: 181 Function: " + func + " Time Stamp: ") != std::string::npos);
		}
		
		{
			const auto source = SOURCE_INFO();
			const auto result = ToString("{0}", source);
			VERIFY(result.find("File: " + file + " Line: 187 Function: " + func) != std::string::npos);
		}
		
		return success;
	}

	bool SourceInfoTester::MutInfoTest()
	{
		{
			const auto source = MUT_SRC_INFO();
			VERIFY(source.file == __FILE__);
			VERIFY(source.line == kFILELINE - 2);
			VERIFY(source.func == __FUNCTION__);
		}
		
		{
			auto source = WMUT_SRC_INFO();
			source.file = source.file.filename();
			VERIFY(source.file == std::filesystem::path(__FILEW__).filename());
			VERIFY(source.line == kFILELINE - 3);
			VERIFY(source.func == __FUNCTIONW__);
		}
		
		return success;
	}

	bool SourceInfoTester::MutInfoNoFuncTest()
	{
		{
			const auto source = MUT_SRC_INFO_NO_FUNC();
			VERIFY(source.file == __FILE__);
			VERIFY(source.line == kFILELINE - 2);
			VERIFY(source.func == "");
		}

		{
			auto source = U16MUT_SRC_INFO_NO_FUNC();
			source.file = source.file.filename();
			VERIFY(source.file == std::filesystem::path(__FILEW__).filename());
			VERIFY(source.line == kFILELINE - 3);
			VERIFY(source.func ==u"");
		}
		
		return success;
	}

	bool SourceInfoTester::MutToStringTest()
	{
		std::string file = __FILE__;
		std::string func = __FUNCTION__;

		{
			const auto source = MUT_SRC_INFO();
			const auto result = ToString("{0:f}", source);
			VERIFY(result == "Line: 151");
		}
		
		return success;
	}
}
#endif
