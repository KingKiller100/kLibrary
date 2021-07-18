#include "pch.hpp"
#include "PerformanceTestBase.hpp"

#ifdef TESTING_ENABLED
#include "PerformanceTestManager.hpp"

#include "../../Utility/String/kSprintf.hpp"

#include <iostream>

namespace kTest::performance
{
	PerformanceTestBase::PerformanceTestBase(const char* name) noexcept
		: name(name)
	{}

	PerformanceTestBase::~PerformanceTestBase()
	{
		ClearAll();
	}

	void PerformanceTestBase::Run() noexcept
	{
		try
		{
			Test();
		}
		catch (const std::exception& e)
		{
			const auto output = klib::kString::Sprintf("%s seems to have triggered an exception!\nException: %s", name, e.what());
			PerformanceTestManager::Get().CollectResult(output);
		}

		Output();
		ClearAll();
	}

	void PerformanceTestBase::Add(PerformanceTestBase* test)
	{
		PerformanceTestManager::Get().Add(test);
	}

	void PerformanceTestBase::SetUpParticipants(const std::vector<std::string_view>& participants) noexcept
	{
		std::string subTestName = participants.front().data();

		auto iter = participants.cbegin();
		++iter;
		for (; iter != participants.cend(); ++iter)
		{
			const auto name = *iter;
			subTestName.append(" vs ");
			subTestName.append(name.data());
		}

		iter = participants.cbegin();
		for (; iter != participants.cend(); ++iter)
			AddSubTest(subTestName, *iter);

		std::cout << "\n\t\tRunning sub-test: \"" << subTestName << "\"";

	}

	void PerformanceTestBase::AddSubTest(const std::string& subTestName, const std::string_view& participant) noexcept
	{
		const auto& subTest = avgTimeResults[subTestName];

		if (subTest.find(subTestName) != subTest.end())
			return;

		avgTimeResults[subTestName][participant.data()] = AverageTime{ 0, 0 };
	}

	void PerformanceTestBase::Output() noexcept
	{
		std::cout << "\n\tCalculating Results..." << "\n";

		for (auto& data : avgTimeResults)
		{
			data.second.erase("");

			const auto& subTestName = data.first;
			auto& internalData = data.second;

			Accolade fastest = { name, std::numeric_limits<int64_t>::max() };
			Accolade slowest = { name, std::numeric_limits<int64_t>::min() };

			for (const auto& res : profilerResults)
			{
				if (internalData.find(res.name) == internalData.end())
					continue;

				const auto speed = res.end - res.start;
				auto& timePair = internalData[res.name];
				timePair.time += speed;
				timePair.count++;

				if (speed < fastest.time)
				{
					fastest.name = res.name;
					fastest.time = speed;
				}

				if (speed > slowest.time)
				{
					slowest.name = res.name;
					slowest.time = speed;
				}
			}

			auto minTime = std::numeric_limits<long double>::max();
			auto maxTime = std::numeric_limits<long double>::min();
			std::string winner;

			for (const auto& values : internalData)
			{
				const auto totalTime = values.second.time;
				const auto count = values.second.count;
				const long double avg = CAST(long double, totalTime) / count;

				const auto currentLowest = avg < minTime ? avg : minTime;

				if (currentLowest < minTime)
				{
					winner = values.first;
					minTime = currentLowest;
				}

				maxTime = avg > maxTime ? avg : maxTime;
			}

			const auto difference = maxTime - minTime;
			const auto percentage = (difference / minTime) * 100;
			const auto percentagef = CAST(float, percentage);

			SendResult(subTestName, winner, percentagef, fastest, slowest);
		}
	}

	void PerformanceTestBase::SendResult(const std::string_view& subTestName, const std::string_view& result
		, const float percentageDifference, const Accolade& fastest, const Accolade& slowest) const noexcept
	{
		using namespace klib;
		using namespace klib::kStopwatch;

		const auto& fastestName = fastest.name;
		const auto& slowestName = slowest.name;

		const auto unitsLong = units::GetUnitsStr<units::Micros>(units::UnitStrLength::LONG);
		const auto unitsShort = units::GetUnitsStr<units::Micros>(units::UnitStrLength::SHORT);

		const auto output = klib::kString::Sprintf(R"(%s: 
	%s is the fastest on average by %.3f%%
	Fastest Instance: %s Time: %d%s (%s)
	Slowest Instance: %s Time: %d%s (%s)

)",
subTestName,
result,
percentageDifference,
fastestName,
fastest.time,
unitsShort,
unitsLong,
slowestName,
slowest.time,
unitsShort,
unitsLong);

		PerformanceTestManager::Get().CollectResult(output);
	}

	void PerformanceTestBase::ClearAll()
	{
		ClearAverageTimes();
		ClearProfilerResults();
	}

	void PerformanceTestBase::ClearProfilerResults()
	{
		profilerResults.clear();
		profilerResults.shrink_to_fit();
	}

	void PerformanceTestBase::ClearAverageTimes()
	{
		for (auto && avg_time_result : avgTimeResults)
		{
			avg_time_result.second.clear();
		}
		avgTimeResults.clear();
	}

	const std::string& PerformanceTestBase::GetName() const
	{
		return name;
	}
}
#endif