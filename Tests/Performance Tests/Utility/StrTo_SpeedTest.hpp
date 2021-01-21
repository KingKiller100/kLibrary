﻿#pragma once

#include "../../../Source/Testing/Performance/PerformanceTestBase.hpp"

namespace kTest::performance::util
{
	class StrToSpeedTest : public PerformanceTestBase
	{
	public:
		StrToSpeedTest() noexcept;
		~StrToSpeedTest() noexcept;

	protected:
		void Test() override;

	private:
		void StrToInt();
		void StrToLongLong();
		void StrToFloat();
		void StrToDouble();
	};
	
}
