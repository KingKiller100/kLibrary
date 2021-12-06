#pragma once
#include "../IncludeTesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class ProfilerTester : public TesterBase
	{
	public:
		ProfilerTester();
		~ProfilerTester() override;

	protected:
		void Prepare() noexcept override;

	private:
		void MillisecondsTest();
		void SecondsTest();
	};
}
#endif
