#pragma once
#include "../../Source Files/Testing/Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class FileSystemTester : public Tester
	{
	public:
		FileSystemTester();
		~FileSystemTester() override;
		
	private:
		void Test() override;

		template<typename Dest, typename Source>
		std::basic_string<Dest> SwitchStringFormat(Source source)
		{
			std::basic_string<Dest> dest;

			for (auto& c : source)
			{
				dest.push_back(Dest(c));
			}

			return dest;
		}
	};
}
#endif
