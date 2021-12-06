
#include "SetUpTests.hpp"

#include "kAddTest.hpp"
#include "TesterManager.hpp"

#include "Performance/PerformanceTestManager.hpp"

// Maths Tests
#include "../../Tests/Maths Tests/Algorithm_Test.hpp"
#include "../../Tests/Maths Tests/Fraction_Test.hpp"
#include "../../Tests/Maths Tests/Matrix_Test.hpp"
#include "../../Tests/Maths Tests/Quaternion_Test.hpp"
#include "../../Tests/Maths Tests/Random_Test.hpp"
#include "../../Tests/Maths Tests/Vectors_Test.hpp"

// Utility
#include "../../Tests/Utility Tests/BitTricks_Test.hpp"
#include "../../Tests/Utility Tests/Calendar_Test.hpp"
#include "../../Tests/Utility Tests/Debug_Test.hpp"
#include "../../Tests/Utility Tests/Enum_Test.hpp"
#include "../../Tests/Utility Tests/FileSystem_Test.hpp"
#include "../../Tests/Utility Tests/Logging_Test.hpp"
#include "../../Tests/Utility Tests/Profiler_Test.hpp"
#include "../../Tests/Utility Tests/SourceInfo_Test.hpp"
#include "../../Tests/Utility Tests/Stopwatch_Test.hpp"
#include "../../Tests/Utility Tests/StringConverter_Test.hpp"
#include "../../Tests/Utility Tests/StringTricks_Test.hpp"
#include "../../Tests/Utility Tests/StringView_Test.hpp"
#include "../../Tests/Utility Tests/ToString_Test.hpp"

// Templates
#include "../../Tests/Templates Tests/BytesStorage_Test.hpp"

// Performance Tests
#include "../../Tests/Performance Tests/Maths/Vector_SpeedTest.hpp"
#include "../../Tests/Performance Tests/Maths/Algorithms_SpeedTest.hpp"
#include "../../Tests/Performance Tests/Utility/ToString_SpeedTest.hpp"
#include "../../Tests/Performance Tests/Utility/StrTo_SpeedTest.hpp"

namespace kTest
{
	namespace performance
	{
		namespace
		{
			void AddUtilityPerformanceTests(PerformanceTestManager& pm)
			{
				// Utility
				AddTest<util::StrToSpeedTest>(pm);
				AddTest<util::ToStringSpeedTest>(pm);
			}

			void AddMathsPerformanceTests(PerformanceTestManager& pm)
			{
				// Maths
				AddTest<maths::AlgorithmsSpeedTest>(pm);
				AddTest<maths::VectorSpeedTest>(pm);
			}
		}

		void InitializeAllPerformanceTests(PerformanceTestManager& pm)
		{
			AddUtilityPerformanceTests(pm);
			AddMathsPerformanceTests(pm);
		}
	}

	void InitializeMathsTests(TesterManager& tm)
	{
		AddTest<maths::FractionTester>(tm);
		AddTest<maths::AlgorithmsTester>(tm);
		AddTest<maths::VectorsTester>(tm);
		AddTest<maths::MatricesTester>(tm);
		AddTest<maths::QuaternionsTester>(tm);
		AddTest<maths::RandomTester>(tm);
	}

	void InitializeUtilityTests(TesterManager& tm, bool includeTimeTests)
	{
		AddTest<utility::BitTricksTester>(tm);
		AddTest<utility::StringConverterTester>(tm);
		AddTest<utility::EnumTester>(tm);
		AddTest<utility::StringManipulationTester>(tm);
		AddTest<utility::FormatToStringTester>(tm);
		AddTest<utility::SourceInfoTester>(tm);
		AddTest<utility::CalendarTester>(tm);
		AddTest<utility::FileSystemTester>(tm);
		AddTest<utility::DebugTester>(tm);
		AddTest<utility::LoggingTester>(tm);
		AddTest<utility::StringViewTester>(tm);
		
		if (includeTimeTests)
		{
			AddTest<utility::StopWatchTester>(tm);
			AddTest<utility::ProfilerTester>(tm);
		}
	}

	void InitializeTemplateTests(TesterManager& tm)
	{
		AddTest<templates::BytesStorageTester>(tm);
	}


	void InitializeAllTests(TesterManager& tm)
	{
		InitializeMathsTests(tm);
		InitializeUtilityTests(tm, true);
		InitializeTemplateTests(tm);
	}
}
