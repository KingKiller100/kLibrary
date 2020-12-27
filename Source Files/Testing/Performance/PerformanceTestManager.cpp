#include "pch.hpp"
#include "PerformanceTestManager.hpp"
#include "PerformanceTestBase.hpp"

#include "../SetUpTests.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

#ifdef TESTING_ENABLED
namespace kTest::performance
{
	PerformanceTestManager::PerformanceTestManager(const Token&)
		: TesterBase("Performance Tests")
	{}

	PerformanceTestManager::~PerformanceTestManager()
		= default;

	void PerformanceTestManager::Initialize()
	{
		namespace fs = std::filesystem;

		path = fs::current_path();
		path += "\\Speed Results\\";
		const auto isMade = fs::create_directory(path);

		if (!fs::exists(path))
		{
			throw std::runtime_error("Test Results path could not be created/found. Please check why!");
		}
		
		const auto endIter = std::filesystem::directory_iterator();
		
		for (auto iter = std::filesystem::directory_iterator(path);
			iter != endIter;
			++iter)
		{
			const auto isFile = iter->is_regular_file();
			if (isFile)
			{
				fs::remove(*iter);
			}
		}

		InitializeTests();
	}

	void PerformanceTestManager::ShutDown()
	{
		ClearAll();
	}

	void PerformanceTestManager::RunAll()
	{
		for (auto& test : tests)
		{
			std::cout << "\n";
			RunTest(test);
			OutputResult(test->GetName());
		}

		std::cout << "\nTests have concluded. Please find results in the following path:\n" << path << std::endl;
		std::cin.get();
	}

	void PerformanceTestManager::RunTest(PerformanceTestBase * test)
	{
		std::cout << "\tNow Testing: " << test->GetName() << " ";

		const auto start = std::clock();
		test->Run();
		const auto end = std::clock();

		const auto duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;
		
		std::cout << "Runtime: " << duration << "s (seconds)\n";
	}

	void PerformanceTestManager::Add(PerformanceTestBase* test)
	{
		tests.insert(test);
	}

	void PerformanceTestManager::ClearAll()
	{
		for (auto& test : tests)
		{
			delete test;
			// test = nullptr;
		}
		tests.clear();
	}

	void PerformanceTestManager::CollectResult(const std::string_view& result)
	{
		results.append(result);
	}

	void PerformanceTestManager::InitializeTests()
	{
		InitializeAllPerformanceTests();
	}

	void PerformanceTestManager::OutputResult(const std::string& name)
	{
		using namespace klib;
		const auto filename = name + ".txt";
		const auto fullPath = path / filename;

		std::ofstream file(fullPath, std::ios::out | std::ios::app);
		file << results;
		file.close();

		results.clear();
	}

	void PerformanceTestManager::Test()
	{
		Initialize();
		RunAll();
		ShutDown();
	}

	PerformanceTestManager& PerformanceTestManager::Get()
	{
		static Token t;
		static PerformanceTestManager instance(t);

		return instance;
	}
}
#endif
