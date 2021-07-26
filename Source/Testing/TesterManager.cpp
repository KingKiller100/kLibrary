#include "pch.hpp"
#include "TesterManager.hpp"

#ifdef TESTING_ENABLED

#include "TesterBase.hpp"

#include "SetUpTests.hpp"

// Speed Testing
#include "Performance/PerformanceTestManager.hpp"

// Change console colour
#include "../Utility/Misc/kConsoleColour.hpp"

#include "../Utility/Thread/kThreadPool.hpp"
#include "../Utility/FileSystem/kFileSystem.hpp"

#include <iostream>
#include <stdexcept>
#include <mutex>
#include <Windows.h>

namespace kTest
{
	using namespace klib;
	using namespace kString;

	TesterManager::TesterManager()
		: success(true)
		, skipPerformanceTests(false)
	{
		std::cout.precision(3);
	}

	TesterManager::~TesterManager()
	{
		Shutdown();
	}

	void TesterManager::Initialize(bool skipPerformanceTesting)
	{
		using namespace klib;

		skipPerformanceTests = skipPerformanceTesting;
		
		kFileSystem::SetCurrentWorkingDirectory(kFileSystem::GetExeDirectory());

		path = std::filesystem::current_path().string() + "\\Test Results\\";
		const auto isMade = std::filesystem::create_directory(path.c_str());

		if (!isMade && !std::filesystem::exists(path))
		{
			throw std::runtime_error("Test results directory could not be created/found. Please check why!");
		}

		path += "Results.txt";
		std::filesystem::remove(path.c_str());

		file.open(path, std::ios::out | std::ios::app);
	}
	
	void TesterManager::Add(TesterBase* test)
	{
		tests.insert(std::unique_ptr<TesterBase>(std::move(test)));
	}

	void TesterManager::RunAll(size_t noOfThreads)
	{
		const auto testCount = tests.size();

		if (testCount == 0)
		{
			std::cout << "TestManager empty\n";
			return;
		}
		const auto maxThreads = (std::min<size_t>)(ULTRA_THREAD_RIPPER, tests.size());
		noOfThreads = std::clamp<size_t>(noOfThreads, SINGLE, maxThreads);

		std::cout << "Testing: " << (noOfThreads > 1 ? "Multi-Threaded" : "Single Threaded") <<
			"[" << noOfThreads << "]" << "\n";

		testTimes.reserve(testCount);

		clock_t start;

		PerformTests(noOfThreads, start);

		const clock_t end = std::clock();

		const auto finalTime = static_cast<double>(end - start) / CLOCKS_PER_SEC;
		const auto secs = static_cast<unsigned>(finalTime);
		const auto remainder = finalTime - secs;
		const unsigned millis = static_cast<unsigned>(std::chrono::milliseconds::period::den * remainder);
		const auto avgTime = GetAverageTime();

		auto timeStr = Sprintf("Total Runtime: %us %ums | ", secs, millis);
		timeStr.append(Sprintf("Average Runtime: %.3fms", avgTime));

		WriteToFile(timeStr);

		std::cout << "\n" << timeStr << "\n";

		std::cout << "\nTests have concluded. Please find results in the following path:\n" << path << std::endl;
	}

	void TesterManager::PerformTests(const size_t noOfThreads, std::clock_t& outStart)
	{
		if (noOfThreads != 0)
		{
			kThread::ThreadPool threads(noOfThreads);

			outStart = std::clock();
			for (const auto& test : tests)
			{
				threads.QueueJob({ [this, &test]
					{
						Run(*test);
					}
					, test->GetName() });
			}
		}
		else
		{
			outStart = std::clock();
			for (const auto& test : tests)
			{
				Run(*test);
			}
		}
	}

	double TesterManager::GetAverageTime() const
	{
		double avgTime(0);

		for (auto t : testTimes)
			avgTime += t;

		avgTime /= testTimes.size();
		return avgTime;
	}

	void TesterManager::RunPerformanceTests() const
	{
		if (!success || skipPerformanceTests)
			return;
		
		auto& test = performance::PerformanceTestManager::Get();
		std::cout << "Now Testing: " << test.GetName() << " ";
		test.Run();
	}

	void TesterManager::Run(TesterBase& test)
	{
		std::string nameOpener("Ran: ");
		nameOpener.append(test.GetName());
		nameOpener.append(" | ");

		const auto start = std::chrono::high_resolution_clock::now();
		const auto pass = test.Run();
		const auto end = std::chrono::high_resolution_clock::now();

		const auto duration = end - start;

		const auto testTime =
			static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()) / 1'000'000;

		testTimes.push_back(testTime);

		const auto resTimeStr = Sprintf("| Runtime: %.3fms"
			, testTime);

		WriteToConsole(pass, nameOpener, resTimeStr);

		if (!pass)
			success = false;

		const auto results = pass
			? Sprintf("Success: %s %s\n\n",
				test.GetName(),
				resTimeStr) // Success Case
			: Sprintf("Failure: %s %s\n%s",
				test.GetName(),
				resTimeStr,
				test.GetFailureData()); // Fail Case

		WriteToFile(results);
	}

	void TesterManager::WriteToConsole(const bool pass, const std::string& nameOpenerStr,
		const std::string& resTimeStr)
	{
		const auto scopeLocker = std::scoped_lock(consoleMutex);

		auto* const hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		std::cout << nameOpenerStr;

		SetConsoleTextAttribute(hConsole, pass
			? kMisc::ConsoleColour::LIGHT_GREEN
			: kMisc::ConsoleColour::SCARLET_RED);

		std::cout << (pass ? "Pass" : "Fail");

		SetConsoleTextAttribute(hConsole, kMisc::ConsoleColour::LIGHT_GREY);

		std::cout << " " << resTimeStr << "\n";
	}

	void TesterManager::WriteToFile(const std::string& results)
	{
		const auto scopeLocker = std::scoped_lock(fileMutex);
		file << results;
		std::flush(file);
	}

	void TesterManager::Shutdown()
	{
		if (file.is_open())
			file.close();

		ClearAllTests();
	}

	void TesterManager::ClearAllTests()
	{
		if (!tests.empty())
			tests.clear();

		testTimes.clear();
	}
}
#endif // TESTING_ENABLED
