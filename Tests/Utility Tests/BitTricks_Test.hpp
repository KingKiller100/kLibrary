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
		void Prepare() noexcept override;

	private:
		void SetTest();
		void ClearTest();
		void CheckTest();
		void ToggleTest();
	};
}
#endif

