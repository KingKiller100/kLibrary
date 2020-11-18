#pragma once

#include "../../../Source Files/Testing/Performance/PerformanceTestBase.hpp"

namespace kTest::performance::util
{ 
	class ToStringSpeedTest : public PerformanceTestBase
	{
	public:
		ToStringSpeedTest() noexcept;
		~ToStringSpeedTest() noexcept;
		
	protected:
		void Test() override;
		void IntTest();
		void DoubleTest();
		void StringTest();

	private:
		void OstreamTest();
		void SprintfTest();
	};
}
