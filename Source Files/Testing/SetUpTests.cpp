﻿#include "pch.hpp"
#include "SetUpTests.hpp"

#include "TesterManager.hpp"
#include "Performance/PerformanceTestManager.hpp"

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

// Performance Tests
#include "../../Tests/Performance Tests/Utility/ToString_SpeedTest.hpp"

#include "../../Tests/Performance Tests/Maths/Vector_SpeedTest.hpp"
#include "../../Tests/Performance Tests/Maths/Algorithms_SpeedTest.hpp"

namespace kTest
{
	namespace performance
	{
		namespace
		{
			void AddUtilityPerformanceTests()
			{
				// Utility
				AddTest<PerformanceTestManager>(new util::ToStringSpeedTest());
			}

			void AddMathsPerformanceTests()
			{
				// Maths
				AddTest<PerformanceTestManager>(new maths::AlgorithmsSpeedTest());
				AddTest<PerformanceTestManager>(new maths::VectorSpeedTest());
			}
		}

		void InitializeAlPerformanceTests()
		{
			AddUtilityPerformanceTests();
			AddMathsPerformanceTests();
		}
	}

	void InitializeMathsTests()
	{
		AddTest<TesterManager>(new maths::FractionTester());
		AddTest<TesterManager>(new maths::AlgorithmsTester());
		AddTest<TesterManager>(new maths::VectorsTester());
		AddTest<TesterManager>(new maths::MatricesTester());
		AddTest<TesterManager>(new maths::QuaternionsTester());
		AddTest<TesterManager>(new maths::RandomTester());
	}

	void InitializeUtilityTests()
	{
		AddTest<TesterManager>(new utility::UTFConverterTester());
		AddTest<TesterManager>(new utility::StringManipulationTester());
		AddTest<TesterManager>(new utility::FormatToStringTester());
		AddTest<TesterManager>(new utility::CalendarTester());
		AddTest<TesterManager>(new utility::FileSystemTester());
		AddTest<TesterManager>(new utility::DebugHelpTester());
		AddTest<TesterManager>(new utility::LoggingTester());
		AddTest<TesterManager>(new utility::StringViewTester());
		AddTest<TesterManager>(new utility::StopWatchTester());
	}

	void InitializeTemplateTests()
	{
		AddTest<TesterManager>(new templates::BytesStorageTester());
	}


	void InitializeAllTests()
	{
		InitializeMathsTests();
		InitializeUtilityTests();
		InitializeTemplateTests();
	}
}