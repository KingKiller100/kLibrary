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
		void InfoTest();
		void MutInfoTest();
		void InfoNoFuncTest();
		void MutInfoNoFuncTest();
		void ToStringTest();
		void MutToStringTest();
	};
}
#endif
