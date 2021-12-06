
#include "Enum_Test.hpp"

#include "../../Source/Utility/Enum/kEnum.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
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

	class TestResults
	{
	public:
		using Underlying_t = std::uint8_t;

		enum Value : std::uint8_t
		{
			A_STAR = (static_cast<size_t>(1) << 0),
			A = (static_cast<size_t>(1) << 1),
			B = (static_cast<size_t>(1) << 2),
			C = (static_cast<size_t>(1) << 3),
			D = (static_cast<size_t>(1) << 4),
			E = (static_cast<size_t>(1) << 5),
			F = (static_cast<size_t>(1) << 6),
			PASS = A_STAR | A | B | C,
			FAIL = D | E | F
		};

	protected:
		struct secret_impl_TestResults
		{
			static constexpr auto size = 9;
			static constexpr std::array<std::uint8_t, size> values = {
				(klib::kEnum::secret::impl::ignore_assign<Underlying_t>)A_STAR = (static_cast<size_t>(1) << 0),
				(klib::kEnum::secret::impl::ignore_assign<Underlying_t>)A = (static_cast<size_t>(1) << 1),
				(klib::kEnum::secret::impl::ignore_assign<Underlying_t>)B = (static_cast<size_t>(1) << 2),
				(klib::kEnum::secret::impl::ignore_assign<Underlying_t>)C = (static_cast<size_t>(1) << 3),
				(klib::kEnum::secret::impl::ignore_assign<Underlying_t>)D = (static_cast<size_t>(1) << 4),
				(klib::kEnum::secret::impl::ignore_assign<Underlying_t>)E = (static_cast<size_t>(1) << 5),
				(klib::kEnum::secret::impl::ignore_assign<Underlying_t>)F = (static_cast<size_t>(1) << 6),
				(klib::kEnum::secret::impl::ignore_assign<Underlying_t>)PASS = A_STAR | A | B | C,
				(klib::kEnum::secret::impl::ignore_assign<Underlying_t>)FAIL = D | E | F,
			};
			static constexpr std::array<std::string_view, size> raw_names = {
				"A_STAR = (static_cast<size_t>(1) << 0)", "A = (static_cast<size_t>(1) << 1)",
				"B = (static_cast<size_t>(1) << 2)", "C = (static_cast<size_t>(1) << 3)",
				"D = (static_cast<size_t>(1) << 4)", "E = (static_cast<size_t>(1) << 5)",
				"F = (static_cast<size_t>(1) << 6)", "PASS = A_STAR | A | B | C", "FAIL = D | E | F",
			};
		};

	public:
		constexpr TestResults(Value value) : value(value)
		{
		}

		constexpr TestResults(std::uint8_t val) : value(val)
		{
			const auto& v = secret_impl_TestResults::values;
			if (std::find(std::begin(v), std::end(v), val) == v.end())
			{
				throw std::out_of_range("Value given is not mapped to a value for " "enum type \"" "TestResults" "\"");
			}
		}

		constexpr TestResults& operator=(const Value value)
		{
			this->value = value;
			return *this;
		}

		constexpr operator Value() const { return static_cast<Value>(value); }
		[[nodiscard]] constexpr Value ToEnum() const { return static_cast<Value>(value); }
		[[nodiscard]] constexpr std::uint8_t ToUnderlying() const { return value; }
		[[nodiscard]] static constexpr size_t Count() noexcept { return secret_impl_TestResults::size; }
		[[nodiscard]] constexpr bool MaskCmp(Value target) const { return MaskCmp(target, true, false); }

		template <typename T1, typename T2, typename = std::enable_if_t<std::is_convertible_v<T2, T1>>>
		[[nodiscard]] constexpr std::decay_t<T1> MaskCmp(Value mask, T1&& successState, T2&& failState) const
		{
			if (mask & value) return successState;
			return failState;
		}

		[[nodiscard]] constexpr bool Compare(Value target) const { return Compare(target, true, false); }

		template <typename T1, typename T2>
		[[nodiscard]] constexpr std::decay_t<T1> Compare(Value target, T1&& successState, T2&& failState) const
		{
			if (target == value) return successState;
			return failState;
		}

		static constexpr TestResults FromString(const std::string_view& search)
		{
			return FromStringImpl(search.data(), 0);
		}

		[[nodiscard]] static std::string PrettyValue(TestResults input)
		{
			static const auto type = std::string(PrettyType()) + "::";
			const auto name = std::string(input.ToString<char>());
			const auto prettyValue = type + name;
			return prettyValue;
		}

		[[nodiscard]] static constexpr std::string_view PrettyType() { return "enum " "TestResults"; }

		template <class Char_t = char>
		[[nodiscard]] std::basic_string_view<Char_t> ToString() const
		{
			const auto name = ToStringImpl<Char_t>();
			return name;
		}

		[[nodiscard]] constexpr bool operator<(const TestResults& other) const { return value < other.value; }
		[[nodiscard]] constexpr bool operator<=(const TestResults& other) const { return value <= other.value; }

		template <typename T>
		[[nodiscard]] constexpr bool operator<(const T& other) const { return value < other; }

		template <typename T>
		[[nodiscard]] constexpr bool operator<=(const T& other) const { return value <= other; }

		static void ForEach(std::function<void(TestResults)> func)
		{
			for (auto v : secret_impl_TestResults::values) { func(v); }
		}

	private:
		template <class Char_t>
		[[nodiscard]] const Char_t* ToStringImpl() const
		{
			const auto* name = TrimmedNames<Char_t>();
			return name;
		}

		template <class Char_t>
		[[nodiscard]] const Char_t* TrimmedNames() const
		{
			static std::unique_ptr<Char_t[]> the_names[secret_impl_TestResults::size];
			static bool initialized = false;
			size_t index = 0;
			if (!initialized)
			{
				InitializeNames<Char_t>(the_names);
				initialized = true;
			}
			do { if (ToEnum() == secret_impl_TestResults::values[index]) break; }
			while (++index < secret_impl_TestResults::size);
			return the_names[index].get();
		}

		template <class Char_t>
		constexpr void InitializeNames(std::unique_ptr<Char_t[]> (&the_names)[secret_impl_TestResults::size]) const
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

		static constexpr Value FromStringImpl(const char* s, size_t index)
		{
			using namespace klib::kEnum::secret::impl;
			if (index >= secret_impl_TestResults::size) std::_Xout_of_range(
				"Name does not map to a value in enum: " "TestResults");
			const auto matches = matches_untrimmed(secret_impl_TestResults::raw_names[index].data(), s);
			const auto ret = matches
				                 ? static_cast<Value>(secret_impl_TestResults::values[index])
				                 : FromStringImpl(s, index + 1);
			return ret;
		}

	private:
		std::uint8_t value;
	};;

	EnumTester::EnumTester()
		: TesterBase("Enum Test")
	{
	}

	EnumTester::~EnumTester()
	= default;

	void EnumTester::Prepare() noexcept
	{
		ADD_TEST(SizeOfTest());
		ADD_TEST(FundamentalTest());
		ADD_TEST(CompareTest());
		ADD_TEST(MaskCmpTest());
		ADD_TEST(ToEnumTest());
		ADD_TEST(ToUnderlyingTest());
		ADD_TEST(ToStringTest());
		ADD_TEST(PrettyTypeTest());
		ADD_TEST(PrettyValueTest());
		ADD_TEST(FromStringTest());
	}

	void EnumTester::FundamentalTest()
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
	}

	void EnumTester::ToStringTest()
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
	}

	void EnumTester::CompareTest()
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
			constexpr auto pass = static_cast<TestResults::Value>(
				TestResults::A_STAR | TestResults::A | TestResults::B | TestResults::C);
			VERIFY_COMPILE_TIME(tr.Compare(pass) == true);
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
	}

	void EnumTester::MaskCmpTest()
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
	}

	void EnumTester::ToEnumTest()
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
	}

	void EnumTester::ToUnderlyingTest()
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
	}

	void EnumTester::PrettyTypeTest()
	{
		constexpr auto prettyType = TestResults::PrettyType();
		VERIFY(prettyType == "enum TestResults");
	}

	void EnumTester::PrettyValueTest()
	{
		{
			const auto prettyVal = TestResults::PrettyValue(TestResults::PASS);
			VERIFY(prettyVal == "enum TestResults::PASS");
		}

		{
			const auto prettyVal = TestResults::PrettyValue(TestResults::FAIL);
			VERIFY(prettyVal == "enum TestResults::FAIL");
		}
	}

	void EnumTester::FromStringTest()
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
	}

	ENUM_CLASS(Bytes1, uint8_t, f);

	ENUM_CLASS(Bytes2, uint16_t, f);

	ENUM_CLASS(Bytes4, uint32_t, f);

	ENUM_CLASS(Bytes8, uint64_t, f);

	void EnumTester::SizeOfTest()
	{
		VERIFY_COMPILE_TIME(sizeof(Bytes1) == 1);
		VERIFY_COMPILE_TIME(sizeof(Bytes2) == 2);
		VERIFY_COMPILE_TIME(sizeof(Bytes4) == 4);
		VERIFY_COMPILE_TIME(sizeof(Bytes8) == 8);
	}
}
#endif
