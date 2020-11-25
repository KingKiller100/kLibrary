#include "pch.hpp"
#include "ToString_SpeedTest.hpp"

#include "../../../Source Files/Utility/String/kToString.hpp"

#include <sstream>
#include <iomanip>

namespace kTest::performance::util
{
	using namespace klib;

	constexpr auto maxIter = static_cast<size_t>(1e3);

	ToStringSpeedTest::ToStringSpeedTest() noexcept
		: PerformanceTestBase("ToString Speed Test")
	{}

	ToStringSpeedTest::~ToStringSpeedTest() noexcept
		= default;

	void ToStringSpeedTest::Test()
	{
		AvgSpeed();
		OstreamSprintfTest();
		kSprintfTest();
		OstreamTest();
		IntTest();
		DoubleTest();
		StringTest();
		SprintfTest();
	}

	void ToStringSpeedTest::kSprintfTest()
	{
		const std::vector<std::string_view> participants = { "klib::kString::ToString double", "klib::kString::SprintfWrapper" };
		SetUpParticipants(participants);

		constexpr auto num = 6.015625;

		const auto str = kString::ToString("{0:b}", 5.5f);

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				(void)kString::ToString("{0:3}", num);
			}

			{
				START_TEST(participants[1]);
				(void)kString::SprintfWrapper("%.3f", num);
			}
		}
	}

	void ToStringSpeedTest::IntTest()
	{
		const std::vector<std::string_view> participants = { "klib::kString::ToString int", "std::stringstream int", "C sprintf int" };
		SetUpParticipants(participants);

		const int sint = -50;

		std::unique_ptr<char[]> buffer;
		std::stringstream ss;

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				const auto output = kString::ToString("{0}", sint);
			}

			{
				START_TEST(participants[1]);
				ss << sint;
			}

			{
				START_TEST(participants[2]);
				const auto length = _snprintf(nullptr, 0, "sint: %d"
					, sint
				);
				buffer.reset(new char[length + 1]);
				sprintf(buffer.get(), "sint: %d", sint);
			}

			ss.clear();
			buffer.release();
		}
	}

	void ToStringSpeedTest::DoubleTest()
	{
		const std::vector<std::string_view> participants = { "klib::kString::ToString double", "std::stringstream double"
			, "C sprintf double", "klib::kString::SprintfWrapper" };
		SetUpParticipants(participants);

		const auto f64 = 125.625;

		std::unique_ptr<char[]> buffer;
		std::stringstream ss;

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				(void)kString::ToString("{0:3}", f64);
			}

			{
				START_TEST(participants[1]);

				ss << std::setprecision(3) << f64;
			}

			constexpr auto format = "f64: %.3f";
			{
				START_TEST(participants[2]);

				const auto length = _snprintf(nullptr, 0, format
					, f64
				);
				buffer.reset(new char[length + 1]);
				sprintf(buffer.get(), format, f64);
			}

			{
				START_TEST(participants[3]);

				(void)kString::SprintfWrapper(format, f64);
			}

			ss.clear();
			buffer.release();
		}
	}

	void ToStringSpeedTest::StringTest()
	{
		const std::vector<std::string_view> participants = { "klib::kString::ToString string", "std::stringstream string"
			, "sprintf string", "klib::kString::SprintfWrapper string" };
		SetUpParticipants(participants);

		const auto str = "string";

		std::unique_ptr<char[]> buffer;
		std::stringstream ss;

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				const auto output = kString::ToString("{0}", str);
			}

			{
				START_TEST(participants[1]);
				ss << str;
			}

			constexpr auto format = "string: %s";
			{
				START_TEST(participants[2]);

				const auto length = _snprintf(nullptr, 0, format, str);
				buffer.reset(new char[length + 1]);
				sprintf(buffer.get(), format, str);
			}

			{
				START_TEST(participants[3]);

				(void)kString::SprintfWrapper(format, str);
			}

			ss.clear();
			buffer.release();
		}
	}

	void ToStringSpeedTest::OstreamSprintfTest()
	{
		const std::vector<std::string_view> participants = { "C sprintf", "std::stringstream" };
		SetUpParticipants(participants);

		const std::string string = "Input1";
		const std::string_view string_view = "Input2";
		const double f64 = 64.625;
		const float f32 = 32.25;
		const size_t ulong = 10000;
		const long long slong = -5000;
		const unsigned uint = 100;
		const int sint = -50;
		const bool boolean = true;
		const auto ptr = std::make_shared<int64_t>(64);

		std::unique_ptr<char[]> buffer;

		std::stringstream ss;
		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[1]);
				const auto length = _snprintf(nullptr, 0, "string: %s - string_view: %s - double: %.5f"
					" - float: %.5f - ulong: %llu - slong: %lld - uint: %u - sint: %d - boolean: %s - Pointer: %p"
					, string.data()
					, string_view.data()
					, f64
					, f32
					, ulong
					, slong
					, uint
					, sint
					, boolean ? "true" : "false"
					, ptr.get()
				);
				buffer.reset(new char[length + 1]);
				sprintf(buffer.get(), "string: %s - string_view: %s - double: %.5f"
					" - float: %.5f - ulong: %llu - slong: %lld - uint: %u - sint: %d - boolean: %s - Pointer: %p"
					, string.data()
					, string_view.data()
					, f64
					, f32
					, ulong
					, slong
					, uint
					, sint
					, boolean ? "true" : "false"
					, ptr.get()
				);
			}

			{
				START_TEST(participants[1]);
				ss << "string: " << string << " - string_view: " << string_view << " - double: " << std::setprecision(5) << f64 <<
					" - float: " << std::setprecision(5) << f32 << " - ulong: " << ulong << " - slong: " << slong << " - uint: " << uint << " - sint: " << sint << " - boolean: " << boolean;
			}
			ss.clear();
			buffer.release();
		}

	}


	void ToStringSpeedTest::OstreamTest()
	{
		const std::vector<std::string_view> participants = { "klib::kString::ToString", "std::stringstream" };
		SetUpParticipants(participants);

		const std::string string = "Input1";
		const std::string_view string_view = "Input2";
		const double f64 = 64.625;
		const float f32 = 32.25;
		const size_t ulong = 10000;
		const long long slong = -5000;
		const unsigned uint = 100;
		const int sint = -50;
		const bool boolean = true;

		std::stringstream ss;
		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				const auto output = kString::ToString("string: {0} - string_view: {1} - double: {2}"
					" - float: {3} - ulong: {4} - slong: {5} - uint: {6} - sint: {7} - boolean: {8}"
					, string
					, string_view
					, f64
					, f32
					, ulong
					, slong
					, uint
					, sint
					, boolean
				);
			}

			{
				START_TEST(participants[1]);
				ss << "string: " << string << " - string_view: " << string_view << " - double: " << std::setprecision(5) << f64 <<
					" - float: " << std::setprecision(5) << f32 << " - ulong: " << ulong << " - slong: " << slong << " - uint: " << uint << " - sint: " << sint << " - boolean: " << boolean;
			}
			ss.clear();
		}

	}

	void ToStringSpeedTest::SprintfTest()
	{
		const std::vector<std::string_view> participants = { "klib::kString::ToString", "C sprintf", "klib::kString::stringify::kSprintfWrapper" };
		SetUpParticipants(participants);

		const std::string string = "Input1";
		const std::string_view string_view = "Input2";
		const double f64 = 64.625;
		const float f32 = 32.25;
		const size_t ulong = 10000;
		const long long slong = -5000;
		const unsigned uint = 100;
		const int sint = -50;
		const bool boolean = true;
		const auto ptr = std::make_shared<int64_t>(64);

		std::unique_ptr<char[]> buffer;
		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				const auto output = kString::ToString("string: {0} - string_view: {1} - double: {2:5}"
					" - float: {3:5} - ulong: {4} - slong: {5} - uint: {6} - sint: {7} - boolean: {8} - Pointer: {9}"
					, string
					, string_view
					, f64
					, f32
					, ulong
					, slong
					, uint
					, sint
					, boolean
					, ptr
				);
			}

			{
				START_TEST(participants[1]);
				const auto length = _snprintf(nullptr, 0, "string: %s - string_view: %s - double: %.5f"
					" - float: %.5f - ulong: %llu - slong: %lld - uint: %u - sint: %d - boolean: %s - Pointer: %p"
					, string.data()
					, string_view.data()
					, f64
					, f32
					, ulong
					, slong
					, uint
					, sint
					, boolean ? "true" : "false"
					, ptr.get()
				);
				buffer.reset(new char[length + 1]);
				sprintf(buffer.get(), "string: %s - string_view: %s - double: %.5f"
					" - float: %.5f - ulong: %llu - slong: %lld - uint: %u - sint: %d - boolean: %s - Pointer: %p"
					, string.data()
					, string_view.data()
					, f64
					, f32
					, ulong
					, slong
					, uint
					, sint
					, boolean ? "true" : "false"
					, ptr.get()
				);
			}



			{
				START_TEST(participants[2]);

				(void)kString::SprintfWrapper("string: %s - string_view: %s - double: %.5f"
					" - float: %.5f - ulong: %llu - slong: %lld - uint: %u - sint: %d - boolean: %s - Pointer: %p"
					, string.data()
					, string_view.data()
					, f64
					, f32
					, ulong
					, slong
					, uint
					, sint
					, boolean ? "true" : "false"
					, ptr
				);
			}

			buffer.release();
		}
	}

	void ToStringSpeedTest::AvgSpeed()
	{
		const std::vector<std::string_view> participants = { "klib::kString::ToString" };
		SetUpParticipants(participants);

		const std::string string = "Input1";
		const std::string_view string_view = "Input2";
		const double f64 = 64.625;
		const float f32 = 32.25;
		const size_t ulong = 10000;
		const long long slong = -5000;
		const unsigned uint = 100;
		const int sint = -50;
		const bool boolean = true;
		const auto ptr = std::make_shared<int64_t>(64);

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				const auto output = kString::ToString("string: {0} - string_view: {1} - double: {2:5}"
					" - float: {3:5} - ulong: {4} - slong: {5} - uint: {6} - sint: {7} - boolean: {8} - Pointer: {9}"
					, string
					, string_view
					, f64
					, f32
					, ulong
					, slong
					, uint
					, sint
					, boolean
					, ptr
				);
			}
		}
	}
}
