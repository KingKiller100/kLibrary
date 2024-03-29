﻿#pragma once

#include "../../IncludePerformanceTesterBase.hpp"


#ifdef TESTING_ENABLED
namespace kTest::performance::maths
{
	class VectorSpeedTest : public PerformanceTestBase
	{
	public:
		explicit VectorSpeedTest();
		~VectorSpeedTest() override;

	protected:
		void Test() override;

	private:
		// floats
		void Vec2f();
		void Vec3f();
		void Vec4f();
		
		// doubles
		void Vec2d();
		void Vec3d();
		void Vec4d();
		
		// BigInt_t
		void Vec2bit();
		void Vec3bit();
		void Vec4bit();
	};			 
}
#endif
