#pragma once
#include "../../Source Files/Testing/Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::templates
{
	class BytesStorageTester final : public Tester
	{
	public:
		BytesStorageTester();
		~BytesStorageTester() override;

	protected:
		void Test() override;

	private:
		bool CharacterTest();
		bool IntegerTest();
		bool SizeTypeTest();
		bool StringTest();
		bool VectorTest();
	};
}
#endif

