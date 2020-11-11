#include "pch.hpp"
#include "PerformanceTestBase.hpp"

#include "PerformanceTestManager.hpp"

#include "../../Source Files/Utility/String/kToString.hpp"

#include <iostream>

#ifdef TESTING_ENABLED
namespace kTest::performance
{
	PerformanceTestBase::PerformanceTestBase(const char* name) noexcept
		: name(name)
	{}

	PerformanceTestBase::~PerformanceTestBase()
		= default;

	void PerformanceTestBase::Run() noexcept
	{
		try
		{
			Test();
		}
		catch (...)
		{
			const auto output = klib::kString::ToString("{0} seems to have crashed!\n", name);
			PerformanceTestManager::Get().CollectResult(output);
		}

		Output();
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
		const auto& subTest = results[subTestName];

		if (subTest.find(subTestName) != subTest.end())
			return;

		results[subTestName][participant.data()] = AverageTime{ 0, 0 };
	}

	void PerformanceTestBase::Output() noexcept
	{
		std::cout << "\nCalculating Results..." << "\n";

		for (auto& data : results)
		{
			data.second.erase("");

			const auto& subTestName = data.first;
			auto& internalData = data.second;

			Accolade fastest = { name, std::numeric_limits<long long>::max() };
			Accolade slowest = { name, std::numeric_limits<long long>::min() };

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

		const auto& fastestName = fastest.name;
		const auto& slowestName = slowest.name;

		const auto output = klib::kString::ToString(R"(%s: 
	%s is the fastest on average by %.3f%%
	Fastest Instance: %s Time: %dus (microseconds)
	Slowest Instance: %s Time: %dus (microseconds)

)", 
			subTestName.data(),
			result.data(), 
			percentageDifference,
			fastestName.data(), 
			fastest.time,
			slowestName.data(),
			slowest.time);

		PerformanceTestManager::Get().CollectResult(output);
	}

	const std::string& PerformanceTestBase::GetName() const
	{
		return name;
	}
}
#endif