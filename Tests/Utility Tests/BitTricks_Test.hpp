#pragma once
#include "../IncludeTesterBase.hpp"

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

