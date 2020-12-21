#include "pch.hpp"
#include "TesterManager.hpp"

#include "TesterBase.hpp"

#include "SetUpTests.hpp"

// Speed Testing
#include "Performance/PerformanceTestManager.hpp"

// Change console colour
#include "../Utility/Misc/kConsoleColour.hpp"

#include "../Utility/Thread/kThreadPool.hpp"
#include "../Utility/FileSystem/kFileSystem.hpp"

#include <iostream>
#include <mutex>
#include <stack>
#include <thread>
#include <Windows.h>


#ifdef TESTING_ENABLED
namespace kTest
{
	using namespace klib;
	using namespace kString;

	TesterManager::TesterManager(Token&)
		: success(true)
	{
	}

	TesterManager::~TesterManager()
	{
		Shutdown();
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
	}

	void TesterManager::Initialize()
	{
		using namespace klib;

		kFileSystem::SetCurrentWorkingDirectory(kFileSystem::GetExeDirectory());
		
		path = std::filesystem::current_path().string() + "\\Test Results\\";
		const auto isMade = std::filesystem::create_directory(path.c_str());

		if (!isMade && !std::filesystem::exists(path))
		{
			throw std::runtime_error("Test Results directory could not be created/found. Please check why!");
		}

		path += "Results.txt";
		std::filesystem::remove(path.c_str());

		file.open(path, std::ios::out | std::ios::app);
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
		timesRecorded.reserve(tests.size());
		std::stack<decltype(tests)::value_type> finishedTests;

		clock_t start;
		{
			kThread::ThreadPool threads;
			const auto size = threads.GetSize();

			start = std::clock();
			while (!tests.empty())
			{
				for (size_t i = 0; i < size && !tests.empty(); ++i)
				{
					const auto& test = tests.front();
					threads.QueueJob({ [this, test] { Run(*test); }, test->GetName() });
					finishedTests.push(test);
					tests.pop_front();
				}
			}
		}
		const auto end = std::clock();

		const auto finalTime = static_cast<double>(end - start) / CLOCKS_PER_SEC;
		const auto secs = CAST(unsigned, finalTime);
		const auto remainder = finalTime - secs;
		const unsigned millis = CAST(unsigned, std::chrono::milliseconds::period::den * remainder);
		const auto avgTime = GetAverageTime();


		auto timeStr = Sprintf("Total Runtime: %us %ums | ", secs, millis);
		timeStr.append(Sprintf("Average Runtime: %.3fms", avgTime));

		WriteToFile(timeStr);

		std::cout << "\n" << timeStr << "\n";

		std::cout << "\nTests have concluded. Please find results in the following path:\n" << path << std::endl;
		std::cin.get();
	}

	double TesterManager::GetAverageTime() const
	{
		double avgTime(0);

		for (auto t : timesRecorded)
			avgTime += t;

		avgTime /= timesRecorded.size();
		return avgTime;
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
		std::string nameOpener("Ran: ");
		nameOpener.append(test.GetName());
		nameOpener.append(" | ");

		const auto start = std::chrono::high_resolution_clock::now();
		const auto pass = test.Run();
		const auto end = std::chrono::high_resolution_clock::now();

		const auto duration = end - start;

		const auto testTime =
			static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()) / 1000000;

		timesRecorded.push_back(testTime);

		const auto resTimeStr = Sprintf("| Runtime: %.3f%s"
			, testTime
			, "ms");

		WriteToConsole(pass, nameOpener, resTimeStr);

		if (!pass)
			success = false;

		const auto results = pass
			? Sprintf("Success: Test Name: %s %s\n\n",
				test.GetName(),
				resTimeStr) // Success Case
			: Sprintf("Failure: Test Name: %s %s\n%s",
				test.GetName(),
				resTimeStr,
				test.GetFailureData()); // Fail Case

		WriteToFile(results);
	}

	void TesterManager::WriteToConsole(const bool pass, const std::string& NameOpenerStr,
		const std::string& resTimeStr)
	{
		auto locker = std::scoped_lock(consoleMutex);

		auto* const hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		std::cout << NameOpenerStr;

		SetConsoleTextAttribute(hConsole, pass
			? kMisc::ConsoleColour::LIGHT_GREEN
			: kMisc::ConsoleColour::SCARLET_RED);

		const auto resultStr = Sprintf("%s", (pass ? "Pass" : "Fail"));
		std::cout << resultStr;
		SetConsoleTextAttribute(hConsole, kMisc::ConsoleColour::LIGHT_GREY);

		std::cout.precision(3);

		std::cout << " " << resTimeStr << "\n";
	}

	void TesterManager::WriteToFile(const std::string& results)
	{
		auto locker = std::scoped_lock(fileMutex);
		file << results;
	}

	TesterManager& TesterManager::Get()
	{
		static Token t;
		static TesterManager instance(t);
		return instance;
	}
}
#endif // TESTING_ENABLED
