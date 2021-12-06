#pragma once

#include "../IncludeTesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class EnumTester final : public TesterBase
	{
	public:
		EnumTester();
		~EnumTester() override;

	protected:
		void Prepare() noexcept override;

	private:
		void FundamentalTest();
		void ToStringTest();
		void SizeOfTest();
		void CompareTest();
		void MaskCmpTest();
		void ToEnumTest();
		void ToUnderlyingTest();
		void PrettyTypeTest();
		void PrettyValueTest();
		void FromStringTest();
	};
}
#endif
