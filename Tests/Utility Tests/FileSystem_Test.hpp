#pragma once
#include "../../Source/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class FileSystemTester : public TesterBase
	{
	public:
		FileSystemTester();
		~FileSystemTester() override;
		
	protected:
		void Test() override;

	private:
		bool FunctionalityTest();
		bool PathToStringTest();
		
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
