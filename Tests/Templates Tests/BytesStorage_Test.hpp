#pragma once
#include "../../Source/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::templates
{
	class BytesStorageTester final : public TesterBase
	{
	public:
		BytesStorageTester();
		~BytesStorageTester() override;

	protected:
		void Prepare() noexcept override;

	private:
		void CharacterTest();
		void IntegerTest();
		void SizeTypeTest();
		void StringTest();
		void VectorTest();
	};
}
#endif

