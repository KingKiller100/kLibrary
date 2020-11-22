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

	ENUM_CLASS(TestResults, std::uint8_t
		, A_STAR = BIT_SHIFT(0)
		, A = BIT_SHIFT(1)
		, B = BIT_SHIFT(2)
		, C = BIT_SHIFT(3)
		, D = BIT_SHIFT(4)
		, E = BIT_SHIFT(5)
		, F = BIT_SHIFT(6)
		, PASS = A_STAR | A | B | C
		, FAIL = D | E | F
	);

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
			const auto tr = TestResults{TestResults::D};
			VERIFY(tr.ToEnum() == TestResults::D);
		}

		{
			const auto tr = TestResults{TestResults::B};
			VERIFY(tr.ToEnum() == TestResults::B);
		}

		{
			const auto tr = TestResults{TestResults::F};
			VERIFY(tr.ToEnum() != TestResults::B);
		}

		{
			const auto tr = TestResults{TestResults::F};
			VERIFY(tr.ToEnum() == TestResults::F);
		}

		return success;
	}

	bool EnumTester::ToUnderlyingTest()
	{
		{
			constexpr auto tr = TestResults{TestResults::F};
			VERIFY(tr.ToUnderlying() == 64);
		}

		{
			constexpr auto tr = TestResults{TestResults::D};
			VERIFY(tr.ToUnderlying() != 64);
		}

		{
			constexpr auto tr = TestResults{TestResults::E};
			VERIFY(tr.ToUnderlying() == 32);
		}

		{
			constexpr auto tr = TestResults{TestResults::E};
			VERIFY(tr.ToUnderlying() != 64);
		}

		{
			constexpr auto tr = TestResults{TestResults::FAIL};
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
		const auto prettyVal = TestResults::PrettyValue(TestResults::PASS);
		VERIFY(prettyVal == "enum TestResults::Pass");

		return success;
	}

	bool EnumTester::FromStringTest()
	{
		const auto fromString = TestResults::FromString("D");
		VERIFY(fromString == TestResults::D);
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
