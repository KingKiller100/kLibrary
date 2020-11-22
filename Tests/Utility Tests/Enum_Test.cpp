#include "pch.hpp"
#include "Enum_Test.hpp"

#include "../../Source Files/Utility/Enum/kEnum.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	// #define BIT_SHIFT(n) 1 << n

	// ENUM_CLASS(TestResults, std::uint8_t
	// 	, A_STAR = BIT_SHIFT(0)
	// 	, A = BIT_SHIFT(1)
	// 	, B = BIT_SHIFT(2)
	// 	, C = BIT_SHIFT(3)
	// 	, D = BIT_SHIFT(4)
	// 	, E = BIT_SHIFT(5)
	// 	, F = BIT_SHIFT(6)
	// 	, PASS = A_STAR | A | B | C
	// 	, FAIL = D | E | F
	// );

	// Revealed the code inside ENUM_CLASS macro

	struct secret_impl_TestResults
	{
		using underlying_t = std::uint8_t;

		enum
		{
			A_STAR = (1 << 0),
			A = (1 << 1),
			B = (1 << 2),
			C = (1 << 3),
			D = (1 << 4),
			E = (1 << 5),
			F = (1 << 6),
			PASS = A_STAR | A | B | C,
			FAIL = D | E | F
		};

		static constexpr auto size = IDENTITY(
			COUNT(A_STAR = (1 << 0), A = (1 << 1), B = (1 << 2), C = (1 << 3), D = (1 << 4), E = (1 << 5), F = (1
				<< 6), PASS = A_STAR | A | B | C, FAIL = D | E | F));
		static constexpr std::array<underlying_t, size> values = {
			IDENTITY(
				IGNORE_ASSIGN(A_STAR = (1 << 0) , A = (1 << 1) , B = (1 << 2) , C = (1 << 3) , D = (1 << 4) , E = (1 <<
					5) , F = (1 << 6) , PASS = A_STAR | A | B | C , FAIL = D | E | F))
		};
		static constexpr std::array<std::string_view, size> raw_names = {
			IDENTITY(
				STRINGIZE(A_STAR = (1 << 0) , A = (1 << 1) , B = (1 << 2) , C = (1 << 3) , D = (1 << 4) , E = (1 << 5) ,
					F = (1 << 6) , PASS = A_STAR | A | B | C , FAIL = D | E | F))
		};
	};

	class TestResults
	{
		using underlying_t = std::uint8_t;
	public:
		enum enum_t : underlying_t
		{
			A_STAR = (1 << 0),
			A = (1 << 1),
			B = (1 << 2),
			C = (1 << 3),
			D = (1 << 4),
			E = (1 << 5),
			F = (1 << 6),
			PASS = A_STAR | A | B | C,
			FAIL = D | E | F
		};

		constexpr TestResults(const enum_t value) : value(value)
		{
		}

		constexpr operator enum_t() const { return static_cast<enum_t>(value); }
		USE_RESULT constexpr enum_t ToEnum() const { return static_cast<enum_t>(value); }
		USE_RESULT constexpr underlying_t ToUnderlying() const { return value; }
		USE_RESULT constexpr bool MaskCmp(enum_t target) const { return MaskCmp(target, true, false); }

		template <typename T1, typename T2, typename = std::enable_if_t<std::is_convertible_v<T2, T1>>>
		USE_RESULT constexpr std::decay_t<T1> MaskCmp(enum_t target, T1&& successState, T2&& failState) const
		{
			if (target & value) return successState;
			return failState;
		}

		USE_RESULT constexpr bool Compare(enum_t target) const { return Compare(target, true, false); }

		template <typename T1, typename T2>
		USE_RESULT constexpr std::decay_t<T1> Compare(enum_t target, T1&& successState, T2&& failState) const
		{
			if (target == value) return successState;
			return failState;
		}

		static constexpr enum_t FromString(const std::string_view& search) { return FromStringImpl(search.data(), 0); }
		USE_RESULT static std::string PrettyValue(TestResults input)
		{
			static const auto type = std::string(PrettyType()) + "::";
			const auto name = std::string(input.ToString<char>());
			const auto prettyValue = type + name;
			return prettyValue;
		}

		USE_RESULT static constexpr std::string_view PrettyType() { return "enum " "TestResults"; }

		template <class Char_t = char>
		USE_RESULT std::basic_string_view<Char_t> ToString() const
		{
			const auto name = ToStringImpl<Char_t>();
			return name;
		}

	private:
		template <class Char_t>
		USE_RESULT const Char_t* ToStringImpl() const
		{
			const auto* name = TrimmedNames<Char_t>();
			return name;
		}

		template <class Char_t>
		USE_RESULT const Char_t* TrimmedNames() const
		{
			static std::unique_ptr<Char_t[]> the_names[secret_impl_TestResults::size];
			static bool initialized = false;
			size_t index = 0;
			if (!initialized)
			{
				InitializeNames<Char_t>(the_names);
				initialized = true;
			}
			do { if (ToEnum() == secret_impl_TestResults::values[index]) break; } while (++index < secret_impl_TestResults::size);
			return the_names[index].get();
		}

		template <class Char_t>
		constexpr void InitializeNames(std::unique_ptr<Char_t[]>(&the_names)[secret_impl_TestResults::size]) const
		{
			using namespace klib::kEnum::secret::impl;
			for (auto i = 0; i < secret_impl_TestResults::size; ++i)
			{
				const auto& raw_name = secret_impl_TestResults::raw_names[i];
				auto& name = the_names[i];
				const auto length_til_terminator = std::strcspn(raw_name.data(), terminators);
				name.reset(new Char_t[length_til_terminator + 1]);
				size_t index(0);
				while (index < length_til_terminator)
				{
					name[index] = Char_t(raw_name[index]);
					++index;
				}
				name[length_til_terminator] = Char_t('\0');
			}
		}

		static constexpr enum_t FromStringImpl(const char* s, size_t index)
		{
			using namespace klib::kEnum::secret::impl;
			if (index >= secret_impl_TestResults::size) std::_Xout_of_range(
				"Name does not map to a value in enum: " "TestResults");
			const auto matches = matches_untrimmed(secret_impl_TestResults::raw_names[index].data(), s);
			const auto ret = matches
				? static_cast<enum_t>(secret_impl_TestResults::values[index])
				: FromStringImpl(s, index + 1);
			return ret;
		}

	private:
		underlying_t value;
	};;

	EnumTester::EnumTester()
		: Tester("Enum Test")
	{
	}

	EnumTester::~EnumTester()
		= default;

	void EnumTester::Test()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(SizeOfTest());
		VERIFY_MULTI(FundamentalTest());
		VERIFY_MULTI(CompareTest());
		VERIFY_MULTI(MaskCmpTest());
		VERIFY_MULTI(ToEnumTest());
		VERIFY_MULTI(ToUnderlyingTest());
		VERIFY_MULTI(ToStringTest());
		VERIFY_MULTI(PrettyTypeTest());
		VERIFY_MULTI(PrettyValueTest());
		VERIFY_MULTI(FromStringTest());
		VERIFY_MULTI_END();
	}

	bool EnumTester::FundamentalTest()
	{
		VERIFY_COMPILE_TIME(TestResults::A_STAR == 1);
		VERIFY_COMPILE_TIME(TestResults::A == 2);
		VERIFY_COMPILE_TIME(TestResults::B == 4);
		VERIFY_COMPILE_TIME(TestResults::C == 8);
		VERIFY_COMPILE_TIME(TestResults::D == 16);
		VERIFY_COMPILE_TIME(TestResults::E == 32);
		VERIFY_COMPILE_TIME(TestResults::F == 64);
		VERIFY_COMPILE_TIME(TestResults::PASS == 15); // 1+2+4+8
		VERIFY_COMPILE_TIME(TestResults::FAIL == 112); // 16+32+64

		return success;
	}

	bool EnumTester::ToStringTest()
	{
		{
			constexpr TestResults tr = TestResults::A_STAR;
			const auto result = tr.ToString();
			VERIFY(result == "A_STAR");
		}

		{
			constexpr TestResults tr = TestResults::B;
			const auto result = tr.ToString();
			VERIFY(result == "B");
		}

		{
			constexpr TestResults tr = TestResults::F;
			const auto result = tr.ToString<char16_t>();
			VERIFY(result == u"F");
		}

		{
			constexpr TestResults tr = TestResults::D;
			const auto result = tr.ToString<char32_t>();
			VERIFY(result == U"D");
		}

		return success;
	}

	bool EnumTester::CompareTest()
	{
		{
			constexpr TestResults tr = TestResults::E;
			VERIFY_COMPILE_TIME(tr.Compare(TestResults::E) == true);
		}

		{
			constexpr TestResults tr = TestResults::PASS;
			VERIFY_COMPILE_TIME(tr.Compare(TestResults::PASS) == true);
		}

		{
			constexpr TestResults tr = TestResults::PASS;
			VERIFY_COMPILE_TIME(tr.Compare(TestResults::FAIL) == false);
		}

		{
			constexpr TestResults tr = TestResults::PASS;
			constexpr auto pass = static_cast<TestResults::enum_t>(
				TestResults::A_STAR | TestResults::A | TestResults::B | TestResults::C);
			VERIFY_COMPILE_TIME(tr.Compare(static_cast<TestResults::enum_t>(pass)) == true);
		}

		{
			constexpr TestResults tr = TestResults::A_STAR;
			constexpr auto cmp = TestResults::A_STAR;
			constexpr std::string_view result = tr.Compare(cmp, "Perfect", "Not Perfect");
			VERIFY_COMPILE_TIME(result == "Perfect");
		}

		{
			constexpr TestResults tr = TestResults::A_STAR;
			constexpr auto cmp = TestResults::C;
			constexpr std::string_view result = tr.Compare(cmp, "Perfect", "Not Perfect");
			VERIFY_COMPILE_TIME(result == "Not Perfect");
		}

		{
			constexpr TestResults tr = TestResults::FAIL;
			constexpr auto cmp = TestResults::C;
			const std::string_view result = tr.Compare(cmp, "Perfect", "Not Perfect");
			VERIFY(result == "Not Perfect");
		}

		return success;
	}

	bool EnumTester::MaskCmpTest()
	{
		{
			constexpr TestResults tr = TestResults::FAIL;
			constexpr auto cmp = TestResults::C;
			VERIFY_COMPILE_TIME(tr.MaskCmp(cmp) == false);
		}

		{
			constexpr TestResults tr = TestResults::FAIL;
			constexpr auto cmp = TestResults::D;
			VERIFY_COMPILE_TIME(tr.MaskCmp(cmp) == true);
		}

		{
			constexpr TestResults tr = TestResults::PASS;
			constexpr auto cmp = TestResults::D;
			VERIFY_COMPILE_TIME(tr.MaskCmp(cmp) == false);
		}

		{
			constexpr TestResults tr = TestResults::PASS;
			constexpr auto cmp = TestResults::A;
			VERIFY_COMPILE_TIME(tr.MaskCmp(cmp) == true);
		}

		{
			constexpr TestResults tr = TestResults::PASS;
			constexpr auto cmp = TestResults::C;
			constexpr std::string_view result = tr.MaskCmp(cmp, "Passed", "Failed");
			VERIFY_COMPILE_TIME(result == "Passed");
		}

		{
			constexpr TestResults tr = TestResults::PASS;
			constexpr auto cmp = TestResults::F;
			const std::string_view result = tr.MaskCmp(cmp, "Passed", "Failed");
			VERIFY(result == "Failed");
		}

		return success;
	}

	bool EnumTester::ToEnumTest()
	{
		{
			const auto tr = TestResults{ TestResults::D };
			VERIFY(tr.ToEnum() == TestResults::D);
		}

		{
			const auto tr = TestResults{ TestResults::B };
			VERIFY(tr.ToEnum() == TestResults::B);
		}

		{
			const auto tr = TestResults{ TestResults::F };
			VERIFY(tr.ToEnum() != TestResults::B);
		}

		{
			const auto tr = TestResults{ TestResults::F };
			VERIFY(tr.ToEnum() == TestResults::F);
		}

		return success;
	}

	bool EnumTester::ToUnderlyingTest()
	{
		{
			constexpr auto tr = TestResults{ TestResults::F };
			VERIFY(tr.ToUnderlying() == 64);
		}

		{
			constexpr auto tr = TestResults{ TestResults::D };
			VERIFY(tr.ToUnderlying() != 64);
		}

		{
			constexpr auto tr = TestResults{ TestResults::E };
			VERIFY(tr.ToUnderlying() == 32);
		}

		{
			constexpr auto tr = TestResults{ TestResults::E };
			VERIFY(tr.ToUnderlying() != 64);
		}

		{
			constexpr auto tr = TestResults{ TestResults::FAIL };
			VERIFY(tr.ToUnderlying() == (16 | 32 | 64));
		}

		return success;
	}

	bool EnumTester::PrettyTypeTest()
	{
		constexpr auto prettyType = TestResults::PrettyType();
		VERIFY(prettyType == "enum TestResults")

			return success;
	}

	bool EnumTester::PrettyValueTest()
	{
		{
			const auto prettyVal = TestResults::PrettyValue(TestResults::PASS);
			VERIFY(prettyVal == "enum TestResults::PASS");
		}

		{
			const auto prettyVal = TestResults::PrettyValue(TestResults::FAIL);
			VERIFY(prettyVal == "enum TestResults::FAIL");
		}

		return success;
	}

	bool EnumTester::FromStringTest()
	{
		{
			constexpr auto fromString = TestResults::FromString("D");
			VERIFY_COMPILE_TIME(fromString == TestResults::D);
		}

		{
			constexpr auto fromString = TestResults::FromString("A_STAR");
			VERIFY_COMPILE_TIME(fromString == TestResults::A_STAR);
		}

		{
			try
			{
				const auto fromString = TestResults::FromString("A*");
				VERIFY(false);
			}
			catch (const std::exception& e)
			{
				const std::string_view expected = "Name does not map to a value in enum: TestResults";
				VERIFY(expected == e.what());
			}
		}

		return success;
	}

	ENUM_CLASS(Bytes1, uint8_t, f);

	ENUM_CLASS(Bytes2, uint16_t, f);

	ENUM_CLASS(Bytes4, uint32_t, f);

	ENUM_CLASS(Bytes8, uint64_t, f);

	bool EnumTester::SizeOfTest()
	{
		VERIFY_COMPILE_TIME(sizeof(Bytes1) == 1);
		VERIFY_COMPILE_TIME(sizeof(Bytes2) == 2);
		VERIFY_COMPILE_TIME(sizeof(Bytes4) == 4);
		VERIFY_COMPILE_TIME(sizeof(Bytes8) == 8);

		return success;
	}
}
#endif
