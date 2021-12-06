#pragma once
#include "../IncludeTesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class FormatToStringTester : public TesterBase
	{
	public:
		FormatToStringTester();
		~FormatToStringTester() override;

	protected:
		void Prepare() noexcept override;

	private:
		void IdentityTest();
		void SprintfWrapperTest();
		void StringifyFloatingPointTest();
		void StringifyHexTest();
		void StringifyBinaryTest();
		void CustomTypeWithToStringTest();
		void CustomTypeWithoutToStringTest();
		void FormatToStringTest();
		void DirectToStringTest();

		void VectorSTL();
	};
}
#endif