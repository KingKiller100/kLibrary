#pragma once
#include "../../Source/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class SourceInfoTester : public TesterBase
	{
	public:
		SourceInfoTester();
		~SourceInfoTester() override;

	protected:
		void Prepare() noexcept override;

	private:
		bool InfoTest();
		bool MutInfoTest();
		bool InfoNoFuncTest();
		bool MutInfoNoFuncTest();
		bool ToStringTest();
		bool MutToStringTest();
	};
}
#endif
