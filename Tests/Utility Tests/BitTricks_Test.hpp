#pragma once
#include "../../Source/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class BitTricksTester final : public TesterBase
	{
	public:
		BitTricksTester();
		~BitTricksTester() override;

	protected:
		void Prepare() override;

	private:
		bool SetTest();
		bool ClearTest();
		bool CheckTest();
		bool ToggleTest();
	};
}
#endif

