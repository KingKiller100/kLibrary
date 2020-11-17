﻿#include "pch.hpp"
#include "ToString_SpeedTest.hpp"

#include "../../../Source Files/Utility/String/kToString.hpp"

#include <sstream>

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
		OstreamTest();
		SprintfTest();
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
		for (auto i =0; i < maxIter; ++i)
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
				ss << "string: " << string << " - string_view: " << string_view << " - double: " << f64 <<
					" - float: " << f32 << " - ulong: " << ulong << " - slong: " << slong << " - uint: " << uint << " - sint: " << sint << " - boolean: " << boolean;
			}
			ss.clear();
		}
		
	}

	void ToStringSpeedTest::SprintfTest()
	{
		const std::vector<std::string_view> participants = { "klib::kString::ToString", "std::sprintf" };
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

		std::unique_ptr<char[]> buffer;
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
				const auto length = _snprintf(nullptr,0, "string: %s - string_view: %s - double: %.5f"
					" - float: %.5f - ulong: %llu - slong: %lld - uint: %u - sint: %d - boolean: %s"
					, string.data()
					, string_view.data()
					, f64
					, f32
					, ulong
					, slong
					, uint
					, sint
					, boolean ? "true" : "false"
				);
				buffer.reset(new char[length + 1]);
				std::sprintf(buffer.get(), "string: %s - string_view: %s - double: %.5f"
					" - float: %.5f - ulong: %llu - slong: %lld - uint: %u - sint: %d - boolean: %s"
					, string.data()
					, string_view.data()
					, f64
					, f32
					, ulong
					, slong
					, uint
					, sint
					, boolean ? "true" : "false"
				);
			}
			buffer.release();
		}
	}
}