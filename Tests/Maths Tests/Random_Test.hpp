#pragma once

#include "../IncludeTesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class RandomTester final : public TesterBase
	{
	public:
		RandomTester() ;
		~RandomTester() override;

	protected:
		void Prepare() noexcept override;

	private:
		void IntegerBCryptTest();
		void IntegerTimeTest();
		void FloatingBCryptPointTest();
		void FloatingTimePointTest();
	};
}
#endif
