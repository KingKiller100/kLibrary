#include "pch.hpp"
#include "SourceInfo_Test.hpp"

#ifdef TESTING_ENABLED
#include "../../Source/Utility/String/kToString.hpp"
#include "../../Source/Utility/String/kStringConverter.hpp"
#include "../../Source/Utility/Debug/Source/kSourceInfo.hpp"
#include "../../Source/Utility/Debug/Source/kMutableSourceInfo.hpp"
#include "../../Source/Utility/Debug/Source/kSourceInfoToString.hpp"
#include "../../Source/Utility/Debug/Source/kMutableSourceInfoToString.hpp"

namespace kTest::utility
{
	SourceInfoTester::SourceInfoTester()
		: TesterBase("Source Code Information Test")
	{
	}

	SourceInfoTester::~SourceInfoTester()
	{
	}

	void SourceInfoTester::Prepare() noexcept
	{
		ADD_TEST(InfoTest());
		ADD_TEST(InfoNoFuncTest());
		ADD_TEST(ToStringTest());
		ADD_TEST(MutInfoTest());
		ADD_TEST(MutInfoNoFuncTest());
		ADD_TEST(MutToStringTest());
	}

	using namespace klib;
	using namespace kDebug;
	using namespace kString;

	void SourceInfoTester::InfoTest()
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
			constexpr auto source = U16SOURCE_INFO();
			VERIFY_COMPILE_TIME(source.file == U16_STR( __FILE__ ));
			VERIFY_COMPILE_TIME(source.line == kFILELINE - 2);
			VERIFY_COMPILE_TIME(source.func == U16_STR( __FUNCTION__ ));
		}

		{
			const auto source = U32SOURCE_INFO();
			VERIFY(source.file == U32_STR( __FILE__ ));
			VERIFY(source.line == kFILELINE - 2);
			VERIFY(source.func == U32_STR( __FUNCTION__ ));
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
			constexpr auto file = u"Test.txt";
			constexpr std::int32_t line = 212;
			constexpr auto func = u"void Test()";
			const auto source = BasicSourceInfo<Char_t>(file, line, func);
			VERIFY(source.file == u"Test.txt");
			VERIFY(source.line == 212);
			VERIFY(source.func == u"void Test()");
		}

		{
			using Char_t = char32_t;
			const std::basic_string<Char_t> file = Convert<Char_t>(__FILE__);
			const std::basic_string<Char_t> func = Convert<Char_t>(__FUNCTION__);
			const std::int32_t line = 73;
			const auto source = BasicSourceInfo<Char_t>(file, line, func);
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
			const auto source = BasicSourceInfo<Char_t>(file, line, func);
			VERIFY(source.file == file);
			VERIFY(source.line == 73);
			VERIFY(source.func == func);
		}
#endif
	}

	void SourceInfoTester::InfoNoFuncTest()
	{
		{
			const auto source = SOURCE_INFO_NO_FUNC();
			VERIFY(source.file == __FILE__);
			VERIFY(source.line == kFILELINE - 2);
			VERIFY(source.func == "");
		}

		{
			constexpr auto source = WSOURCE_INFO_NO_FUNC();
			VERIFY_COMPILE_TIME(source.file == __FILEW__);
			VERIFY_COMPILE_TIME(source.line == kFILELINE - 2);
			VERIFY_COMPILE_TIME(source.func == L"");
		}
	}

	void SourceInfoTester::ToStringTest()
	{
		std::string file = __FILE__;
		std::string func = __FUNCTION__;

		{
			const auto source = SOURCE_INFO();
			const auto result = ToString("{0:l}", source);
			VERIFY(result == "147");
		}

		{
			const auto source = SOURCE_INFO();
			const auto result = ToString("{0:f}", source);
			VERIFY(result == file);
		}

		{
			const auto source = SOURCE_INFO();
			const auto result = ToString("{0:fl}", source);
			VERIFY(result == file + " [159]");
		}

		{
			const auto source = SOURCE_INFO();
			const auto result = ToString("{0:z}", source);
			VERIFY(result == func);
		}

		{
			const auto source = SOURCE_INFO();
			const auto result = ToString("{0:t}", source);
			VERIFY(!result.empty());
		}

		{
			const auto source = SOURCE_INFO();
			const auto result = ToString("{0:a}", source);
			VERIFY(result.find("File: " + file + " Line: 177 Function: " + func) != std::string::npos);
		}

		{
			const auto source = SOURCE_INFO();
			const auto result = ToString("{0}", source);
			VERIFY(result.find("File: " + file + " Line: 183 Function: " + func) != std::string::npos);
		}
	}

	void SourceInfoTester::MutInfoTest()
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
	}

	void SourceInfoTester::MutInfoNoFuncTest()
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
	}

	void SourceInfoTester::MutToStringTest()
	{
		std::wstring file = __FILEW__;
		std::wstring func = __FUNCTIONW__;

		{
			const auto source = MUT_SRC_INFO();
			const auto result = ToString("{0:l}", source);
			VERIFY(result == "237");
		}

		{
			const auto source = WMUT_SRC_INFO();
			const auto result = ToString(L"{0:f}", source);
			VERIFY(result == file);
		}

		{
			const auto source = WMUT_SRC_INFO();
			const auto result = ToString(L"{0:z}", source);
			VERIFY(result == func);
		}

		{
			const auto source = WMUT_SRC_INFO();
			const auto result = ToString(L"{0:t}", source);
			VERIFY(!result.empty());
		}

		{
			const auto source = WSOURCE_INFO();
			const auto result = ToString(L"{0:a}", source);
			VERIFY(result.find(L"File: " + file + L" Line: 261 Function: " + func) != std::string::npos);
		}

		{
			const auto source = WSOURCE_INFO();
			const auto result = ToString(L"{0}", source);
			VERIFY(result.find(L"File: " + file + L" Line: 267 Function: " + func) != std::string::npos);
		}
	}
}
#endif
