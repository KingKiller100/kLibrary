#pragma once

#include "../IncludeTesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class StringConverterTester : public TesterBase
	{
	public:
		StringConverterTester();
		~StringConverterTester() override;

	protected:
		void Prepare() noexcept override;

	private:
		void ConvertUTF16ToUTF32();
		void ConvertASCIIToUTF16();
		void ConvertASCIIToUTF32();
		void ConvertASCIIToUTF8();
	};
}
#endif
