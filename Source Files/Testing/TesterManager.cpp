#include "pch.hpp"
#include "TesterManager.hpp"

#include "Tester.hpp"

#include "SetUpTests.hpp"

// Speed Testing
#include "Performance/PerformanceTestManager.hpp"

// File System to output test results
#include "../Utility/FileSystem/kFileSystem.hpp"

// Times the length of the test
#include "../Utility/Stopwatch/kStopwatch.hpp"

// Change console colour
#include "../Utility/Misc/ConsoleColour.hpp"

#include <iostream>


#ifdef TESTING_ENABLED
namespace kTest
{
	using namespace klib;
	using namespace kStopwatch;
	using namespace kString;

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
		if (!testsSet.empty())
			testsSet.clear();
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

	void TesterManager::Add(Tester* test)
	{
		testsSet.insert(std::unique_ptr<Tester>(std::move(test)));
	}

	void TesterManager::RunAll()
	{
		HighAccuracyStopwatch totalRunTimeTimer("Total Test Run Time");
		timesRecorded.reserve(testsSet.size());

		for (const auto& test : testsSet)
		{
			Run(*test);
		}

		const auto finalTime = totalRunTimeTimer.GetLifeTime<units::Secs>();
		double avgTime(0);

		for (auto t : timesRecorded)
			avgTime += t;

		avgTime /= timesRecorded.size();
		
		const auto secs = CAST(unsigned, finalTime);
		const auto remainder = finalTime - secs;
		const unsigned millis = CAST(unsigned
			, units::Secs::Period_t::den * remainder);

		auto timeStr = stringify::SprintfWrapper("Total Runtime: %us %ums | ", secs, millis);
		timeStr.append(stringify::SprintfWrapper("Average Runtime: %.3fms", avgTime));
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

	void TesterManager::Run(Tester& test)
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
			? stringify::SprintfWrapper("Success: Test Name: %s %s\n\n",
				test.GetName(),
				runtimeResultStr) // Success Case
			: stringify::SprintfWrapper("Failure: Test Name: %s %s\n%s",
				test.GetName(),
				runtimeResultStr,
				test.GetFailureData()); // Fail Case

		kFileSystem::WriteFile(path.c_str(), resultTest.c_str());
	}

	std::string TesterManager::WriteResults(const bool pass, const double resTime) const
	{
		using namespace kString;

		auto* const hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, pass
			? kMisc::ConsoleColour::LIGHT_GREEN
			: kMisc::ConsoleColour::SCARLET_RED);

		auto resultStr = stringify::SprintfWrapper("%s", (pass ? "Pass" : "Fail"));
		std::cout << resultStr;
		SetConsoleTextAttribute(hConsole, 7);

		resultStr.insert(0, "| ");

		auto runtimeResultStr = stringify::SprintfWrapper("| Runtime: %.3f%s"
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
