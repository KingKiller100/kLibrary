#pragma once
#include "../IncludeTesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class StringViewTester : public TesterBase
	{
	public:
		StringViewTester();
		~StringViewTester() override;

	protected:
		void Prepare() noexcept override;

	private:
		void FullTest();
	};

}
#endif

