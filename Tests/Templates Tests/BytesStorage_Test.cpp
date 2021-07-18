#include "pch.hpp"
#include "BytesStorage_Test.hpp"

#include "../../Source/Template/kByteStorager.hpp"

#include "../../Source/Maths/Vectors/Vector2.hpp"
#include "../../Source/Maths/Vectors/Vector3.hpp"


#ifdef TESTING_ENABLED
namespace kTest::templates
{
	BytesStorageTester::BytesStorageTester()
		: TesterBase("Bytes Storage Test")
	{}

	BytesStorageTester::~BytesStorageTester()
		= default;

	using namespace klib::kTemplate;
	using namespace kmaths;
	
	void BytesStorageTester::Prepare() noexcept
	{
		VERIFY_MULTI_INIT();
		//VERIFY_MULTI(CharacterTest());
		VERIFY_MULTI(IntegerTest());
		VERIFY_MULTI(SizeTypeTest());
		VERIFY_MULTI(VectorTest());
		VERIFY_MULTI_END();
	}

	bool BytesStorageTester::CharacterTest()
	{
		using TestType = char;

		{
			constexpr auto volume = 100;
			constexpr auto d_s_t = BytesUnits::BYTE;

			BytesStorage<TestType, volume, d_s_t> storage;
			constexpr auto typeSize = sizeof(TestType);
			constexpr auto storageTypeValue = static_cast<size_t>(d_s_t);

			constexpr auto typeBytes = storage.GetTypeSize();
			VERIFY_COMPILE_TIME(typeSize == typeBytes);

			constexpr auto sType = storage.GetStorageType();
			VERIFY_COMPILE_TIME(sType == d_s_t);

			constexpr auto sValue = storage.GetStorageValue();
			VERIFY(sValue == storageTypeValue);

			constexpr auto maxBytes = storage.GetMaxBytes();
			VERIFY_COMPILE_TIME(maxBytes == (volume * storageTypeValue));

			constexpr auto size = storage.GetLength();
			VERIFY_COMPILE_TIME(size == (volume * storageTypeValue) / typeSize);
		}

		{
			constexpr auto volume = 10;
			constexpr auto d_s_t = BytesUnits::KIBI;

			BytesStorage<TestType, volume, d_s_t> storage;
			constexpr auto typeSize = sizeof(TestType);
			constexpr auto storageTypeValue = static_cast<size_t>(d_s_t);

			constexpr auto typeBytes = storage.GetTypeSize();
			VERIFY_COMPILE_TIME(typeSize == typeBytes);

			constexpr auto sType = storage.GetStorageType();
			VERIFY_COMPILE_TIME(sType == d_s_t);

			constexpr auto sValue = storage.GetStorageValue();
			VERIFY(sValue == storageTypeValue);

			constexpr auto maxBytes = storage.GetMaxBytes();
			VERIFY_COMPILE_TIME(maxBytes == (volume * storageTypeValue));

			constexpr auto size = storage.GetLength();
			VERIFY_COMPILE_TIME(size == (volume * storageTypeValue) / typeSize);
		}

		{
			constexpr auto volume = 5;
			constexpr auto d_s_t = BytesUnits::MEGA;

			BytesStorage<TestType, volume, d_s_t> storage;
			constexpr auto typeSize = sizeof(TestType);
			constexpr auto storageTypeValue = static_cast<size_t>(d_s_t);

			const auto typeBytes = storage.GetTypeSize();
			VERIFY(typeSize == typeBytes);

			const auto sType = storage.GetStorageType();
			VERIFY(sType == d_s_t);

			const auto sValue = storage.GetStorageValue();
			VERIFY(sValue == storageTypeValue);

			const auto maxBytes = storage.GetMaxBytes();
			VERIFY(maxBytes == (volume * storageTypeValue));

			const auto size = storage.GetLength();
			VERIFY(size == (volume * storageTypeValue) / typeSize);
		}


		
	}

	bool BytesStorageTester::IntegerTest()
	{
		using TestType = int;

		{
			constexpr auto volume = 100;
			constexpr auto d_s_t = BytesUnits::BYTE;

			BytesStorage<TestType, volume, d_s_t> storage;
			constexpr auto typeSize = sizeof(TestType);
			constexpr auto storageTypeValue = static_cast<size_t>(d_s_t);

			constexpr auto typeBytes = storage.GetTypeSize();
			VERIFY_COMPILE_TIME(typeSize == typeBytes);

			constexpr auto sType = storage.GetStorageType();
			VERIFY_COMPILE_TIME(sType == d_s_t);

			constexpr auto sValue = storage.GetStorageValue();
			VERIFY(sValue == storageTypeValue);

			constexpr auto maxBytes = storage.GetMaxBytes();
			VERIFY_COMPILE_TIME(maxBytes == (volume * storageTypeValue));

			constexpr auto size = storage.GetLength();
			VERIFY_COMPILE_TIME(size == (volume * storageTypeValue) / typeSize);
		}

		{
			constexpr auto volume = 10;
			constexpr auto d_s_t = BytesUnits::BYTE;

			BytesStorage<TestType, volume, d_s_t> storage;
			constexpr auto typeSize = sizeof(TestType);
			constexpr auto storageTypeValue = static_cast<size_t>(d_s_t);

			constexpr auto typeBytes = storage.GetTypeSize();
			VERIFY_COMPILE_TIME(typeSize == typeBytes);

			constexpr auto sType = storage.GetStorageType();
			VERIFY_COMPILE_TIME(sType == d_s_t);

			constexpr auto sValue = storage.GetStorageValue();
			VERIFY(sValue == storageTypeValue);

			constexpr auto maxBytes = storage.GetMaxBytes();
			VERIFY_COMPILE_TIME(maxBytes == (volume * storageTypeValue));

			constexpr auto size = storage.GetLength();
			VERIFY_COMPILE_TIME(size == (volume * storageTypeValue) / typeSize);
		}

		{
			constexpr auto volume = 1;
			constexpr auto d_s_t = BytesUnits::KILO;

			BytesStorage<TestType, volume, d_s_t> storage;
			constexpr auto typeSize = sizeof(TestType);
			constexpr auto storageTypeValue = static_cast<size_t>(d_s_t);

			const auto typeBytes = storage.GetTypeSize();
			VERIFY(typeSize == typeBytes);

			const auto sType = storage.GetStorageType();
			VERIFY(sType == d_s_t);

			const auto sValue = storage.GetStorageValue();
			VERIFY(sValue == storageTypeValue);

			const auto maxBytes = storage.GetMaxBytes();
			VERIFY(maxBytes == (volume * storageTypeValue));

			const auto size = storage.GetLength();
			VERIFY(size == (volume * storageTypeValue) / typeSize);
		}

		
	}

	bool BytesStorageTester::SizeTypeTest()
	{
		using TestType = size_t;

		{
			constexpr auto volume = 100;
			constexpr auto d_s_t = BytesUnits::BYTE;

			BytesStorage<TestType, volume, d_s_t> storage;
			constexpr auto typeSize = sizeof(TestType);
			constexpr auto storageTypeValue = static_cast<size_t>(d_s_t);

			constexpr auto typeBytes = storage.GetTypeSize();
			VERIFY_COMPILE_TIME(typeSize == typeBytes);

			constexpr auto sType = storage.GetStorageType();
			VERIFY_COMPILE_TIME(sType == d_s_t);

			constexpr auto sValue = storage.GetStorageValue();
			VERIFY(sValue == storageTypeValue);

			constexpr auto maxBytes = storage.GetMaxBytes();
			VERIFY_COMPILE_TIME(maxBytes == (volume * storageTypeValue));

			constexpr auto size = storage.GetLength();
			VERIFY_COMPILE_TIME(size == (volume * storageTypeValue) / typeSize);
		}

		{
			constexpr auto volume = 10;
			constexpr auto d_s_t = BytesUnits::BYTE;

			BytesStorage<TestType, volume, d_s_t> storage;
			constexpr auto typeSize = sizeof(TestType);
			constexpr auto storageTypeValue = static_cast<size_t>(d_s_t);

			constexpr auto typeBytes = storage.GetTypeSize();
			VERIFY_COMPILE_TIME(typeSize == typeBytes);

			constexpr auto sType = storage.GetStorageType();
			VERIFY_COMPILE_TIME(sType == d_s_t);

			constexpr auto sValue = storage.GetStorageValue();
			VERIFY(sValue == storageTypeValue);

			constexpr auto maxBytes = storage.GetMaxBytes();
			VERIFY_COMPILE_TIME(maxBytes == (volume * storageTypeValue));

			constexpr auto size = storage.GetLength();
			VERIFY_COMPILE_TIME(size == (volume * storageTypeValue) / typeSize);
		}

		{
			constexpr auto volume = 1;
			constexpr auto d_s_t = BytesUnits::KILO;

			BytesStorage<TestType, volume, d_s_t> storage;
			constexpr auto typeSize = sizeof(TestType);
			constexpr auto storageTypeValue = static_cast<size_t>(d_s_t);

			constexpr auto typeBytes = storage.GetTypeSize();
			VERIFY(typeSize == typeBytes);

			constexpr auto sType = storage.GetStorageType();
			VERIFY(sType == d_s_t);

			constexpr auto sValue = storage.GetStorageValue();
			VERIFY(sValue == storageTypeValue);

			constexpr auto maxBytes = storage.GetMaxBytes();
			VERIFY(maxBytes == (volume * storageTypeValue));

			constexpr auto size = storage.GetLength();
			VERIFY(size == (volume * storageTypeValue) / typeSize);
		}

		
	}

	bool BytesStorageTester::StringTest()
	{

		using TestType = std::wstring;
		constexpr auto typeSize = sizeof(TestType);

		{
			constexpr auto volume = 100;
			constexpr auto d_s_t = BytesUnits::BYTE;

			BytesStorage<TestType, volume, d_s_t> storage;
			constexpr auto storageTypeValue = static_cast<size_t>(d_s_t);

			constexpr auto typeBytes = storage.GetTypeSize();
			VERIFY_COMPILE_TIME(typeSize == typeBytes);

			constexpr auto sType = storage.GetStorageType();
			VERIFY_COMPILE_TIME(sType == d_s_t);

			constexpr auto sValue = storage.GetStorageValue();
			VERIFY(sValue == storageTypeValue);

			constexpr auto maxBytes = storage.GetMaxBytes();
			VERIFY_COMPILE_TIME(maxBytes == (volume * storageTypeValue));

			constexpr auto size = storage.GetLength();
			VERIFY_COMPILE_TIME(size == (volume * storageTypeValue) / typeSize);
		}

		{
			constexpr auto volume = 10;
			constexpr auto d_s_t = BytesUnits::KILO;

			BytesStorage<TestType, volume, d_s_t> storage;
			constexpr auto storageTypeValue = static_cast<size_t>(d_s_t);

			constexpr auto typeBytes = storage.GetTypeSize();
			VERIFY_COMPILE_TIME(typeSize == typeBytes);

			constexpr auto sType = storage.GetStorageType();
			VERIFY_COMPILE_TIME(sType == d_s_t);

			constexpr auto sValue = storage.GetStorageValue();
			VERIFY(sValue == storageTypeValue);

			constexpr auto maxBytes = storage.GetMaxBytes();
			VERIFY_COMPILE_TIME(maxBytes == (volume * storageTypeValue));

			constexpr auto size = storage.GetLength();
			VERIFY_COMPILE_TIME(size == (volume * storageTypeValue) / typeSize);
		}

		{
			constexpr auto volume = 1;
			constexpr auto d_s_t = BytesUnits::KIBI;

			BytesStorage<TestType, volume, d_s_t> storage;
			constexpr auto storageTypeValue = static_cast<size_t>(d_s_t);

			const auto typeBytes = storage.GetTypeSize();
			VERIFY(typeSize == typeBytes);

			const auto sType = storage.GetStorageType();
			VERIFY(sType == d_s_t);

			const auto sValue = storage.GetStorageValue();
			VERIFY(sValue == storageTypeValue);

			const auto maxBytes = storage.GetMaxBytes();
			VERIFY(maxBytes == (volume * storageTypeValue));

			const auto size = storage.GetLength();
			VERIFY(size == (volume * storageTypeValue) / typeSize);
		}

		
	}

	bool BytesStorageTester::VectorTest()
	{

		{
			using TestType = Vector<double, 50>;
			constexpr auto volume = 100;
			constexpr auto d_s_t = BytesUnits::BYTE;

			BytesStorage<TestType, volume, d_s_t> storage;
			constexpr auto typeSize = sizeof(TestType);
			constexpr auto storageTypeValue = static_cast<size_t>(d_s_t);

			constexpr auto typeBytes = storage.GetTypeSize();
			VERIFY_COMPILE_TIME(typeSize == typeBytes);

			constexpr auto sType = storage.GetStorageType();
			VERIFY_COMPILE_TIME(sType == d_s_t);

			constexpr auto sValue = storage.GetStorageValue();
			VERIFY(sValue == storageTypeValue);

			constexpr auto maxBytes = storage.GetMaxBytes();
			VERIFY_COMPILE_TIME(maxBytes == (volume * storageTypeValue));

			constexpr auto size = storage.GetLength();
			VERIFY_COMPILE_TIME(size == (volume * storageTypeValue) / typeSize);
		}

		{
			using TestType = Vector3f;
			constexpr auto volume = 10;
			constexpr auto d_s_t = BytesUnits::BYTE;

			BytesStorage<TestType, volume, d_s_t> storage;
			constexpr auto typeSize = sizeof(TestType);
			constexpr auto storageTypeValue = static_cast<size_t>(d_s_t);

			constexpr auto typeBytes = storage.GetTypeSize();
			VERIFY_COMPILE_TIME(typeSize == typeBytes);

			constexpr auto sType = storage.GetStorageType();
			VERIFY_COMPILE_TIME(sType == d_s_t);

			constexpr auto sValue = storage.GetStorageValue();
			VERIFY(sValue == storageTypeValue);

			constexpr auto maxBytes = storage.GetMaxBytes();
			VERIFY_COMPILE_TIME(maxBytes == (volume * storageTypeValue));

			constexpr auto size = storage.GetLength();
			VERIFY_COMPILE_TIME(size == (volume * storageTypeValue) / typeSize);
		}

		{
			using TestType = Vector2u;
			constexpr auto volume = 1;
			constexpr auto d_s_t = BytesUnits::BYTE;

			BytesStorage<TestType, volume, d_s_t> storage;
			constexpr auto typeSize = sizeof(TestType);
			constexpr auto storageTypeValue = static_cast<size_t>(d_s_t);

			const auto typeBytes = storage.GetTypeSize();
			VERIFY(typeSize == typeBytes);

			const auto sType = storage.GetStorageType();
			VERIFY(sType == d_s_t);

			const auto sValue = storage.GetStorageValue();
			VERIFY(sValue == storageTypeValue);

			const auto maxBytes = storage.GetMaxBytes();
			VERIFY(maxBytes == (volume * storageTypeValue));

			const auto size = storage.GetLength();
			VERIFY(size == (volume * storageTypeValue) / typeSize);
		}

		
	}
}
#endif 
