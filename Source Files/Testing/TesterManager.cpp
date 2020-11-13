#include "pch.hpp"
#include "TesterManager.hpp"

#include "Tester.hpp"

// Maths Tests
#include "../../Tests/Maths Tests/Random_Test.hpp"
#include "../../Tests/Maths Tests/Matrix_Test.hpp"
#include "../../Tests/Maths Tests/Vectors_Test.hpp"
#include "../../Tests/Maths Tests/Fraction_Test.hpp"
#include "../../Tests/Maths Tests/Algorithm_Test.hpp"
#include "../../Tests/Maths Tests/Quaternion_Test.hpp"

// Utility
#include "../../Tests/Utility Tests/Stopwatch_Test.hpp"
#include "../../Tests/Utility Tests/Logging_Test.hpp"
#include "../../Tests/Utility Tests/Calendar_Test.hpp"
#include "../../Tests/Utility Tests/DebugHelp_Test.hpp"
#include "../../Tests/Utility Tests/FileSystem_Test.hpp"
#include "../../Tests/Utility Tests/StringView_Test.hpp"
#include "../../Tests/Utility Tests/StringConverter_Test.hpp"
#include "../../Tests/Utility Tests/StringManipulation_Test.hpp"
#include "../../Tests/Utility Tests/FormatToString_Test.hpp"

// Templates
#include "../../Tests/Templates Tests/BytesStorage_Test.hpp"

// Speed Testing
#include "../../Tests/Performance Tests/PerformanceTestManager.hpp"

// File System to output test results
#include "../Utility/File System/kFileSystem.hpp"

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
		= default;

	void TesterManager::Shutdown()
	{
		ClearAllTests();
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

	void TesterManager::InitializeMathsTests()
	{
		Add(new maths::FractionTester());
		Add(new maths::AlgorithmsTester());
		Add(new maths::VectorsTester());
		Add(new maths::MatricesTester());
		Add(new maths::QuaternionsTester());
		Add(new maths::RandomTester());
	}

	void TesterManager::InitializeUtilityTests()
	{
		Add(new utility::UTFConverterTester());
		Add(new utility::StringManipulationTester());
		Add(new utility::FormatToStringTester());
		Add(new utility::CalendarTester());
		Add(new utility::FileSystemTester());
		Add(new utility::DebugHelpTester());
		Add(new utility::LoggingTester());
		Add(new utility::StringViewTester());
		Add(new utility::StopWatchTester());
	}

	void TesterManager::InitializeTemplateTests()
	{
		Add(new templates::BytesStorageTester());
	}

	void TesterManager::RunPerformanceTests() const
	{
		if (success)
		{
			std::cout << std::endl;
			auto& test = performance::PerformanceTestManager::Get();
			std::cout << "Now Testing: " << test.GetName() << " ";
			test.Run();
		}
	}

	void TesterManager::Add(Tester* test)
	{
		testsSet.insert(std::unique_ptr<Tester>(std::move(test)));
	}

	void TesterManager::Run(Tester& test)
	{
		auto* const hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		const HighAccuracyStopwatch timer("Test Run Time");

		std::cout << "Now running: " << test.GetName();
		
		const auto pass = test.Run();
		const auto testTime = timer.GetLifeTime<units::Micros>();
		
		if (!pass)
			success = false;

		const auto runtimeResultStr
			= WriteResults(pass, testTime);
		
		const auto resultTest = pass
			? ToString("Success: Test Name: {0} {1}\n\n", 
				test.GetName(),
				runtimeResultStr) // Success Case
			: ToString("Failure: Test Name: {0} {1}\n{2}",
				test.GetName(), 
				runtimeResultStr, 
				test.GetFailureData()); // Fail Case

		kFileSystem::WriteFile(path.c_str(), resultTest.c_str());
	}

	std::string TesterManager::WriteResults(const bool pass, const double resTime) const
	{
		using namespace kString;
				
		auto* const hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		std::cout << " | ";
		
		SetConsoleTextAttribute(hConsole, pass 
			? kMisc::ConsoleColour::LIGHT_GREEN
			: kMisc::ConsoleColour::SCARLET_RED);
		
		auto resultStr = ToString<char>((pass ? "Pass" : "Fail"));
		std::cout << resultStr;
		SetConsoleTextAttribute(hConsole, 7);

		resultStr.insert(0, "| ");

		auto runtimeResultStr = ToString(
			"| Runtime: %.fus (microseconds)", resTime);
		std::cout << " " << runtimeResultStr << "\n";

		runtimeResultStr.insert(0, resultStr + " ");

		return runtimeResultStr;
	}

	void TesterManager::RunAll()
	{
		HighAccuracyStopwatch totalRunTimeTimer("Total Test Run Time");

		for (const auto& test : testsSet)
		{
			Run(*test);
		}

		const auto finalTime = totalRunTimeTimer.GetDeltaTime<units::Secs>();
		const auto secs = CAST(unsigned, finalTime);
		const auto remainder = finalTime - secs;
		const unsigned millis = CAST(unsigned
			, std::chrono::milliseconds::period::den * remainder);

		const auto finalTimeStr = ToString("Total Runtime: {0}s  {1}ms", secs, millis);
		kFileSystem::WriteFile(path, finalTimeStr);

		std::cout << "\n" << finalTimeStr << "\n";

		std::cout << "\nTests have concluded. Please find results in the following path:\n" << path << std::endl;
		std::cin.get();
	}

	void TesterManager::ClearAllTests()
	{
		testsSet.clear();
	}

	TesterManager& TesterManager::Get()
	{
		static Token t;
		static TesterManager instance(t);
		return instance;
	}
}
#endif // TESTING_ENABLED
