#include "pch.hpp"
#include "StringManipulation_Test.hpp"

#include "../../Source Files/Utility/String/kStringManipulation.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	StringManipulationTester::StringManipulationTester()
		: Tester("String Manipulator Test")
	{}

	StringManipulationTester::~StringManipulationTester()
		= default;

	void StringManipulationTester::Test()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(ToWriterAndToReaderTest());
		VERIFY_MULTI(ToLower());
		VERIFY_MULTI(ToUpper());
		VERIFY_MULTI(ReplaceTest());
		VERIFY_MULTI(SplitTest());
		VERIFY_MULTI_END();
	}

	bool StringManipulationTester::ReplaceTest()
	{
		const auto text = klib::kString::StringWriter<char>("My name is Rob");
		const auto res = klib::kString::Replace(text, 'R', 'B');
		VERIFY(res.compare("My name is Bob") == 0);

		return success;
	}

	bool StringManipulationTester::SplitTest()
	{
		const klib::kString::StringWriter<char32_t> poem = U"The wheels on the bus go round and round";

		const auto splits = klib::kString::Split(poem, U" ");
		VERIFY(splits[0].compare(U"The") == 0);
		VERIFY(splits[1].compare(U"wheels") == 0);
		VERIFY(splits[2].compare(U"on") == 0);
		VERIFY(splits[3].compare(U"the") == 0);
		VERIFY(splits[4].compare(U"bus") == 0);
		VERIFY(splits[5].compare(U"go") == 0);
		VERIFY(splits[6].compare(U"round") == 0);
		VERIFY(splits[7].compare(U"and") == 0);
		VERIFY(splits[8].compare(U"round") == 0);

		const auto splitsWithSpace = klib::kString::Split(poem, U" ", klib::kString::PreserveToken::YES);
		VERIFY(splitsWithSpace[0].compare(U"The ") == 0);
		VERIFY(splitsWithSpace[1].compare(U"wheels ") == 0);
		VERIFY(splitsWithSpace[2].compare(U"on ") == 0);
		VERIFY(splitsWithSpace[3].compare(U"the ") == 0);
		VERIFY(splitsWithSpace[4].compare(U"bus ") == 0);
		VERIFY(splitsWithSpace[5].compare(U"go ") == 0);
		VERIFY(splitsWithSpace[6].compare(U"round ") == 0);
		VERIFY(splitsWithSpace[7].compare(U"and ") == 0);
		VERIFY(splitsWithSpace[8].compare(U"round") == 0);

		return success;
	}

	bool StringManipulationTester::ToLower()
	{
		{
			constexpr char text[] = "EXAMPLE TEXT";
			const auto res = klib::kString::ToLower(text);
			constexpr std::string_view expected = "example text";
			VERIFY(expected == res);
		}

		{
			constexpr auto character = u8'X';
			constexpr auto res = klib::kString::ToLower(character);
			constexpr auto expected = u8'x';
			VERIFY_COMPILE_TIME(expected == res);
		}

		{
			constexpr std::wstring_view text = L"EXAMPLE TEXT";
			const auto res = klib::kString::ToLower(text);
			constexpr std::wstring_view expected = L"example text";
			VERIFY(expected == res);
		}

		{
			const std::u16string text = u"EXAMPLE TEXT";
			const auto res = klib::kString::ToLower(text);
			const std::u16string expected = u"example text";
			VERIFY(expected == res);
		}

		{
			constexpr const char32_t* text = U"EXAMPLE TEXT";
			const auto res = klib::kString::ToLower(text);
			constexpr std::u32string_view expected = U"example text";
			VERIFY(expected == res);
		}

		return success;
	}

	bool StringManipulationTester::ToUpper()
	{
		{
			constexpr char text[] = "example text 345";
			const auto res = klib::kString::ToUpper(text);
			constexpr std::string_view expected = "EXAMPLE TEXT 345";
			VERIFY(expected == res);
		}

		{
			constexpr auto character = u8'u';
			constexpr auto res = klib::kString::ToUpper(character);
			constexpr auto expected = u8'U';
			VERIFY(expected == res);
		}

		{
			constexpr std::wstring_view text = L"example text";
			const auto res = klib::kString::ToUpper(text);
			constexpr std::wstring_view expected = L"EXAMPLE TEXT";
			VERIFY(expected == res);
		}

		{
			const std::u16string text = u"example text";
			const auto res = klib::kString::ToUpper(text);
			const std::u16string expected = u"EXAMPLE TEXT";
			VERIFY(expected == res);
		}

		{
			constexpr const char32_t* text = U"example text";
			const auto res = klib::kString::ToUpper(text);
			constexpr std::u32string_view expected = U"EXAMPLE TEXT";
			VERIFY(expected == res);
		}
		
		return success;
	}


	bool StringManipulationTester::ToWriterAndToReaderTest()
	{
		{
			constexpr auto string = "String";

			const auto writer = klib::kString::ToWriter(string);
			const auto reader = klib::kString::ToReader(string);

			VERIFY(writer == string);
			VERIFY(reader == string);

			using CharType = ONLY_TYPE(decltype(string));
			using WriterType = ONLY_TYPE(decltype(writer));
			using ReaderType = ONLY_TYPE(decltype(reader));

			constexpr auto sameWriterType = std::is_same_v<WriterType, std::basic_string<CharType>>;
			constexpr auto sameReaderType = std::is_same_v<ReaderType, std::basic_string_view<CharType>>;
			constexpr auto not_match_W_R = !std::is_same_v<WriterType, ReaderType>;
			VERIFY_COMPILE_TIME(sameWriterType);
			VERIFY_COMPILE_TIME(sameReaderType);
			VERIFY_COMPILE_TIME(not_match_W_R);
		}

		{
			constexpr auto string = L"String";

			const auto writer = klib::kString::ToWriter(string);
			const auto reader = klib::kString::ToReader(string);

			VERIFY(writer == string);
			VERIFY(reader == string);

			using CharType = ONLY_TYPE(decltype(string));
			using WriterType = ONLY_TYPE(decltype(writer));
			using ReaderType = ONLY_TYPE(decltype(reader));

			constexpr auto sameWriterType = std::is_same_v<WriterType, std::basic_string<CharType>>;
			constexpr auto sameReaderType = std::is_same_v<ReaderType, std::basic_string_view<CharType>>;
			constexpr auto not_match_W_R = !std::is_same_v<WriterType, ReaderType>;
			VERIFY_COMPILE_TIME(sameWriterType);
			VERIFY_COMPILE_TIME(sameReaderType);
			VERIFY_COMPILE_TIME(not_match_W_R);
		}

		{
			constexpr auto string = u"String";

			const auto writer = klib::kString::ToWriter(string);
			const auto reader = klib::kString::ToReader(string);

			VERIFY(writer == string);
			VERIFY(reader == string);

			using CharType = ONLY_TYPE(decltype(string));
			using WriterType = ONLY_TYPE(decltype(writer));
			using ReaderType = ONLY_TYPE(decltype(reader));

			constexpr auto sameWriterType = std::is_same_v<WriterType, std::basic_string<CharType>>;
			constexpr auto sameReaderType = std::is_same_v<ReaderType, std::basic_string_view<CharType>>;
			constexpr auto not_match_W_R = !std::is_same_v<WriterType, ReaderType>;
			VERIFY_COMPILE_TIME(sameWriterType);
			VERIFY_COMPILE_TIME(sameReaderType);
			VERIFY_COMPILE_TIME(not_match_W_R);
		}

		{
			constexpr auto string = U"String";

			const auto writer = klib::kString::ToWriter(string);
			const auto reader = klib::kString::ToReader(string);

			VERIFY(writer == string);
			VERIFY(reader == string);

			using CharType = ONLY_TYPE(decltype(string));
			using WriterType = ONLY_TYPE(decltype(writer));
			using ReaderType = ONLY_TYPE(decltype(reader));

			constexpr auto sameWriterType = std::is_same_v<WriterType, std::basic_string<CharType>>;
			constexpr auto sameReaderType = std::is_same_v<ReaderType, std::basic_string_view<CharType>>;
			constexpr auto not_match_W_R = !std::is_same_v<WriterType, ReaderType>;
			VERIFY_COMPILE_TIME(sameWriterType);
			VERIFY_COMPILE_TIME(sameReaderType);
			VERIFY_COMPILE_TIME(not_match_W_R);
		}

#ifdef __cpp_char8_t
		{
			constexpr auto string = u8"String";

			const auto writer = klib::kString::ToWriter(string);
			const auto reader = klib::kString::ToReader(string);

			VERIFY(writer == string);
			VERIFY(reader == string);

			using CharType = ONLY_TYPE(decltype(string));
			using WriterType = ONLY_TYPE(decltype(writer));
			using ReaderType = ONLY_TYPE(decltype(reader));

			constexpr auto sameWriterType = std::is_same_v<WriterType, std::basic_string<CharType>>;
			constexpr auto sameReaderType = std::is_same_v<ReaderType, std::basic_string_view<CharType>>;
			constexpr auto not_match_W_R = !std::is_same_v<WriterType, ReaderType>;
			VERIFY_COMPILE_TIME(sameWriterType);
			VERIFY_COMPILE_TIME(sameReaderType);
			VERIFY_COMPILE_TIME(not_match_W_R);
		}
#endif 

		return success;

	}
}
#endif