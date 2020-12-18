#include "pch.hpp"
#include "TesterManager.hpp"

#include "TesterBase.hpp"

#include "SetUpTests.hpp"

// Speed Testing
#include "Performance/PerformanceTestManager.hpp"

// File System to output test results
#include "../Utility/FileSystem/kFileSystem.hpp"

// Times the length of the test
#include "../Utility/Stopwatch/kStopwatch.hpp"

// Change console colour
#include "../Utility/Misc/kConsoleColour.hpp"

#include <iostream>
#include <mutex>
#include <stack>
#include <thread>


#ifdef TESTING_ENABLED
namespace kTest
{
	using namespace klib;
	using namespace kStopwatch;
	using namespace kString;

	namespace
	{
		std::mutex g_TestManagerOutputMutex;
	}

	TesterManager::TesterManager(Token&)
		: success(true)
	{
		const auto before = kFileSystem::GetCurrentWorkingDirectory();
		const auto ewd = kFileSystem::GetExeDirectory();
		const auto dirChanged = kFileSystem::SetCurrentWorkingDirectory(ewd);

		if (!dirChanged)
			std::runtime_error("Failed to change current directory");

		const auto after = kFileSystem::GetCurrentWorkingDirectory();

		if (before == after)
			std::runtime_error("no change occurred");
	}

	TesterManager::~TesterManager()
	{
		Shutdown();
	}

	void TesterManager::Shutdown()
	{
		ClearAllTests();
	}

	void TesterManager::ClearAllTests()
	{
		if (!tests.empty())
			tests.clear();
	}

	void TesterManager::Initialize()
	{
		using namespace klib;

		path = std::filesystem::current_path().string() + "\\Test Results\\";
		const auto isMade = std::filesystem::create_directory(path.c_str());

		if (!isMade && !std::filesystem::exists(path))
		{
			throw std::runtime_error("Test Results directory could not be created/found. Please check why!");
		}

		path += "Results.txt";
		std::filesystem::remove(path.c_str());
	}

	void TesterManager::InitializeMaths() const
	{
		InitializeMathsTests();
	}

	void TesterManager::InitializeUtility() const
	{
		InitializeUtilityTests();
	}

	void TesterManager::InitializeTemplates() const
	{
		InitializeTemplateTests();
	}

	void TesterManager::Add(TesterBase* test)
	{
		tests.push_back(std::unique_ptr<TesterBase>(std::move(test)));
	}

	void TesterManager::RunAll()
	{
		const size_t numOfThreads = std::thread::hardware_concurrency();

		std::vector<std::thread> threads;

		const HighAccuracyStopwatch timer("Total Test Run Time");
		timesRecorded.reserve(tests.size());
		std::stack<std::shared_ptr<TesterBase>> finishedTests;
		while (!tests.empty())
		{
			for (size_t i = 0; i < numOfThreads && !tests.empty(); ++i)
			{
				const auto& test = tests.front();
				
				if (threads.empty())
				{
					threads.emplace_back(&TesterManager::Run, this, std::ref(*test));
				}
				else
				{
					threads[i] = std::move(std::thread(&TesterManager::Run, this, std::ref(*test)));
				}
				
				finishedTests.push(test);
				tests.pop_front();
			}

			for (auto&& thread : threads)
			{
				thread.join();
			}
		}

		const auto finalTime = timer.GetLifeTime<units::Secs>();
		double avgTime(0);

		for (auto t : timesRecorded)
			avgTime += t;

		avgTime /= timesRecorded.size();

		const auto secs = CAST(unsigned, finalTime);
		const auto remainder = finalTime - secs;
		const unsigned millis = CAST(unsigned
			, units::Secs::Period_t::den * remainder);

		auto timeStr = Sprintf("Total Runtime: %us %ums | ", secs, millis);
		timeStr.append(Sprintf("Average Runtime: %.3fms", avgTime));
		kFileSystem::WriteFile(path, timeStr);

		std::cout << "\n" << timeStr << "\n";

		std::cout << "\nTests have concluded. Please find results in the following path:\n" << path << std::endl;
		std::cin.get();
	}

	void TesterManager::RunPerformanceTests() const
	{
		if (!success)
			return;

		std::cout << std::endl;
		auto& test = performance::PerformanceTestManager::Get();
		std::cout << "Now Testing: " << test.GetName() << " ";
		test.Run();
	}

	void TesterManager::Run(TesterBase& test)
	{
		auto* const hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		const HighAccuracyStopwatch timer("Test Run Time");

		std::cout << "Now running: " << test.GetName() << " | ";

		const auto pass = test.Run();
		const auto testTime = timer.GetLifeTime<units::Millis>();

		timesRecorded.push_back(testTime);

		if (!pass)
			success = false;

		const auto runtimeResultStr
			= WriteResults(pass, testTime);

		const auto resultTest = pass
			? Sprintf("Success: Test Name: %s %s\n\n",
				test.GetName(),
				runtimeResultStr) // Success Case
			: Sprintf("Failure: Test Name: %s %s\n%s",
				test.GetName(),
				runtimeResultStr,
				test.GetFailureData()); // Fail Case

		kFileSystem::WriteFile(path.c_str(), resultTest.c_str());
	}

	std::string TesterManager::WriteResults(const bool pass, const double resTime) const
	{
		auto lock = std::lock_guard(g_TestManagerOutputMutex);

		auto* const hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, pass
			? kMisc::ConsoleColour::LIGHT_GREEN
			: kMisc::ConsoleColour::SCARLET_RED);

		auto resultStr = Sprintf("%s", (pass ? "Pass" : "Fail"));
		std::cout << resultStr;
		SetConsoleTextAttribute(hConsole, 7);

		resultStr.insert(0, "| ");

		auto runtimeResultStr = Sprintf("| Runtime: %.3f%s"
			, resTime
			, units::GetUnitsStr<units::Millis>());
		std::cout << " " << runtimeResultStr << "\n";

		runtimeResultStr.insert(0, resultStr + " ");

		return runtimeResultStr;
	}

	TesterManager& TesterManager::Get()
	{
		static Token t;
		static TesterManager instance(t);
		return instance;
	}
}
#endif // TESTING_ENABLED
