#pragma once

#include "../../IncludePerformanceTesterBase.hpp"

namespace kTest::performance::util
{ 
	class ToStringSpeedTest : public PerformanceTestBase
	{
	public:
		ToStringSpeedTest() noexcept;
		~ToStringSpeedTest() noexcept;
		
	protected:
		void Test() override;
		
	private:
		void kSprintfTest();
		void IntTest();
		void DoubleTest();
		void FloatTest();
		void StringTest();

		void OstreamSprintfTest();
		void OstreamTest();
		void SprintfTest();
		
		void AvgSpeed();
	};
}
