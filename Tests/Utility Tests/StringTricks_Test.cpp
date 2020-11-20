﻿#include "pch.hpp"
#include "StringTricks_Test.hpp"

#include "../../Source Files/Utility/String/kStringTricks.hpp"

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
		VERIFY_MULTI(RemoveTest());
		VERIFY_MULTI(StrToTest());
		VERIFY_MULTI(CountTest());
		VERIFY_MULTI(ReplaceTest());
		VERIFY_MULTI(SplitTest());
		VERIFY_MULTI(IsWhiteSpaceOrNullTest());
		VERIFY_MULTI_END();
	}

	using namespace klib::kString;

	bool StringManipulationTester::StrToTest()
	{
		{
			const std::string str = "1000";
			const auto res = StrTo<int>(str);
			constexpr auto expected = 1000;
			VERIFY(expected == res);
		}

		{
			const std::string str = "750";
			const auto res = StrTo<int>(str);
			constexpr auto expected = 750;
			VERIFY(expected == res);
		}

		{
			const std::wstring str = L"-650";
			const auto res = StrTo<int>(str);
			constexpr auto expected = -650;
			VERIFY(expected == res);
		}

		{
			const std::u16string str = u"8000000000";
			const auto res = StrTo<size_t>(str);
			constexpr auto expected = 8000000000;
			VERIFY(expected == res);
		}

		{
			const std::u16string str = u"27";
			const auto res = StrTo<unsigned char>(str);
			constexpr auto expected = 27;
			VERIFY(expected == res);
		}

		{
			using Int_t = unsigned short;
			const std::u16string str = u"65536"; // 1 more than ushort max
			const auto res = StrTo<Int_t>(str);
			constexpr Int_t expected = std::numeric_limits<Int_t>::max() + 1;
			VERIFY(expected == res);
		}

		{
			using Int_t = unsigned char;
			const std::u16string str = u"300"; // 255 + 45
			const auto res = StrTo<Int_t>(str);
			constexpr Int_t expected = std::numeric_limits<Int_t>::max() + 45;
			VERIFY(expected == res);
		}

		{
			success = false;
			try
			{
				using Int_t = unsigned short;
				const std::u16string str = u"300000"; // Too large to represent as an ushort
				const auto res = StrTo<Int_t>(str);
			}
			catch (...)
			{
				success = true;
			}
		}

		{ // Won't compile due to StrTo only taking integral types
			// const std::u16string str = u"80.5";
			//const auto res = StrTo<double>(str);
			// constexpr auto expected = 1000;
			// VERIFY(expected == res);
		}

		return success;
	}

	bool StringManipulationTester::CountTest()
	{
		{
			constexpr std::string_view test("Aquarium");
			const auto count = Count(test, 'a');
			VERIFY(count == 2);
		}

		{
			constexpr std::string_view test("Aquarium");
			const auto count = Count(test, 'a', 2);
			VERIFY(count == 1);
		}

		{
			constexpr std::string_view test("Aquarium");
			const auto count = Count(test, 'a', 0, CaseSensitive::YES);
			VERIFY(count == 1);
		}

		{
			constexpr std::u32string_view test(U"Douglas");
			const auto count = Count(test, U'g');
			VERIFY(count == 1);
		}

		{
			constexpr std::u16string_view test(u"Onomatopoeia");
			const auto count = Count(test, u'o');
			VERIFY(count == 4);
		}

		{
			constexpr std::wstring_view test(L"His cat and my cat are friends");
			const auto count = Count(test, L"cat");
			VERIFY(count == 2);
		}

		{
			constexpr std::wstring_view test(L"Randall");
			const auto count = Count(test, L'q');
			VERIFY(count == 0);
		}

		return success;
	}

	bool StringManipulationTester::ReplaceTest()
	{
		const auto text = klib::kString::StringWriter<char>("My name is Rob");
		const auto res = Replace(text, 'R', 'B');
		VERIFY(res.compare("My name is Bob") == 0);

		return success;
	}

	bool StringManipulationTester::RemoveTest()
	{
		{
			std::string test("test");
			const auto removed = Remove(test, 'e');
			VERIFY(removed);
			VERIFY(test == "tst");
		}

		{
			std::string test("Boy, why do you have to be such a huge dick");
			const auto removed = Remove(test, "to be");
			VERIFY(removed);
			VERIFY(test == "Boy, why do you have such a huge dick");
		}

		{
			std::string test("the brown fox jumped over the black zoo fence");
			const auto removed = Remove(test, "zoom");
			VERIFY(!removed);
			VERIFY(test == "the brown fox jumped over the black zoo fence");
		}

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

	bool StringManipulationTester::IsWhiteSpaceOrNullTest()
	{
		const auto h = klib::type_trait::Is_CString_V<const char*>;
		
		{
			constexpr char* str = nullptr;
			const auto result = IsWhiteSpaceOrNull(str);
			VERIFY(result == true);
		}
		
		{
			constexpr char str[] = "   ";
			const auto result = IsWhiteSpaceOrNull(str);
			VERIFY(result == true);
		}
		
		return success;
	}
}
#endif