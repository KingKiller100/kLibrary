#pragma once
#include "../PerformanceTestBase.hpp"


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
		void OstreamTest();
		void SprintfTest();
	};
}
