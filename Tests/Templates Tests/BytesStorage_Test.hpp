﻿#pragma once
#include "../../Source/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::templates
{
	class BytesStorageTester final : public TesterBase
	{
	public:
		BytesStorageTester();
		~BytesStorageTester() override;

	protected:
		void Test() override;

	private:
		bool CharacterTest();
		bool IntegerTest();
		bool SizeTypeTest();
		bool StringTest();
		bool VectorTest();
	};
}
#endif

