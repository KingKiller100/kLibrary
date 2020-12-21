#pragma once

#include "../../HelperMacros.hpp"

#include "../../Utility/Profiler/kProfiler.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#ifdef TESTING_ENABLED
namespace kTest::performance
{
	struct AverageTime
	{
		std::int64_t time = 0;
		int count = 0;
	};

	struct Accolade
	{
		std::string name;
		std::int64_t time;
	};

	class PerformanceTestBase
	{
	public:
		PerformanceTestBase(const char* name) noexcept;
		virtual ~PerformanceTestBase();

		void Run() noexcept;

		const std::string& GetName() const;

	protected:
		virtual void Test() = 0;

		void Add(PerformanceTestBase* test);
		void SetUpParticipants(const std::vector<std::string_view>& participants) noexcept;

	private:
		void Output() noexcept;
		void AddSubTest(const std::string& subTestName, const std::string_view& participant) noexcept;
		void SendResult(const std::string_view& subTestName, const std::string_view& result
			, const float percentageDifference, const Accolade& fastest, const Accolade& slowest) const noexcept;

	protected:
		std::string name;
		std::vector<klib::kProfiler::ProfilerResult<std::int64_t>> profilerResults;
		std::map<const std::string, std::map<std::string, AverageTime>> results;
	};
}

#define REPORT_BACK_FUNC [&](const klib::kProfiler::ProfilerResult<std::int64_t>& res)\
		{\
			profilerResults.push_back(res);\
		}\

#define START_TEST(profilee) auto profiler##__LINE__ = klib::kProfiler::Profiler<std::int64_t, klib::kStopwatch::units::Millis> (profilee, REPORT_BACK_FUNC);

#endif
