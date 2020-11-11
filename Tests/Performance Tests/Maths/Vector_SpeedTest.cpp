#include "pch.hpp"
#include "Vector_SpeedTest.hpp"

#include "../../../Source Files/Maths/Vectors/PredefinedVectors.hpp"


#ifdef TESTING_ENABLED
namespace kTest::performance::maths
{
	template<typename T>
	struct Vec
	{
		explicit Vec(const T _x = T(), const T _y = T(), const T _z = T(), const T _w = T())
		{
			const auto ptr = (T*)&vec;

			ptr[0] = _x;
			ptr[1] = _y;
			ptr[2] = _z;
			ptr[3] = _w;
		}

		GETTER_CONSTEXPR(T, X, ((T*)&vec)[0])
		CONST_GETTER_CONSTEXPR(T, X, ((T*)&vec)[0])

		GETTER_CONSTEXPR(T, Y, ((T*)&vec)[1])
		CONST_GETTER_CONSTEXPR(T, Y, ((T*)&vec)[1])

		GETTER_CONSTEXPR(T, Z, ((T*)&vec)[2])
		CONST_GETTER_CONSTEXPR(T, Z, ((T*)&vec)[2])

		GETTER_CONSTEXPR(T, W, ((T*)&vec)[3])
		CONST_GETTER_CONSTEXPR(T, W, ((T*)&vec)[3])

	public:
		kmaths::Big_Int_Type vec[sizeof(kmaths::Big_Int_Type) == sizeof(T) ? 4
			: (sizeof(kmaths::Big_Int_Type) / 2) == sizeof(T) ? 2
			: 1]{};
	};

	VectorSpeedTest::VectorSpeedTest()
		: PerformanceTestBase("Vector Speed Test")
	{}

	VectorSpeedTest::~VectorSpeedTest()
		= default;

	constexpr auto maxIter = kmaths::Big_Int_Type(1e3);

	using namespace kmaths;
	void VectorSpeedTest::Test()
	{
		Vec2f();
		Vec3f();
		Vec4f();
		
		Vec2d();
		Vec3d();
		Vec4d();
		
		Vec2bit();
		Vec3bit();
		Vec4bit();
	}

	void VectorSpeedTest::Vec2f()
	{
		const std::vector<std::string_view> participants = { "kmaths::Vector2f", "Tightly Packed Vector" };
		SetUpParticipants(participants);

		Vector2f kv2(5, 10);
		Vec<float> v2(5, 10);

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				kv2.X() = kv2.X()++;
				kv2.Y() = kv2.Y()++;
			}

			{
				START_TEST(participants[1]);
				v2.X() = v2.X()++;
				v2.Y() = v2.Y()++;
			}
		}
	}

	void VectorSpeedTest::Vec3f()
	{
		const std::vector<std::string_view> participants = { "kmaths::Vector3f", "Tightly Packed Vector" };
		SetUpParticipants(participants);

		Vec<float> v2(5, 10, 15);
		Vector3f kv2(5, 10, 15);

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				kv2.X() = kv2.X()++;
				kv2.Y() = kv2.Y()++;
				kv2.Z() = kv2.Z()++;
			}

			{
				START_TEST(participants[1]);
				v2.X() = v2.X()++;
				v2.Y() = v2.Y()++;
				v2.Z() = v2.Z()++;
			}
		}
	}

	void VectorSpeedTest::Vec4f()
	{
		const std::vector<std::string_view> participants = { "kmaths::Vector4f", "Tightly Packed Vector" };
		SetUpParticipants(participants);

		Vec<float> v2(5, 10, 15, 20);
		Vector4f kv2(5, 10, 15, 20);

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				kv2.X() = kv2.X()++;
				kv2.Y() = kv2.Y()++;
				kv2.Z() = kv2.Z()++;
				kv2.W() = kv2.W()++;
			}

			{
				START_TEST(participants[1]);
				v2.X() = v2.X()++;
				v2.Y() = v2.Y()++;
				v2.Z() = v2.Z()++;
				v2.W() = v2.W()++;
			}
		}
	}

	void VectorSpeedTest::Vec2d()
	{
		const std::vector<std::string_view> participants = { "kmaths::Vector2d", "Tightly Packed Vector" };
		SetUpParticipants(participants);

		Vector2d kv2(5, 10);
		Vec<double> v2(5, 10);

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				kv2.X() = kv2.X()++;
				kv2.Y() = kv2.Y()++;
			}

			{
				START_TEST(participants[1]);
				v2.X() = v2.X()++;
				v2.Y() = v2.Y()++;
			}
		}
	}

	void VectorSpeedTest::Vec3d()
	{
		const std::vector<std::string_view> participants = { "kmaths::Vector3d", "Tightly Packed Vector" };
		SetUpParticipants(participants);

		Vector3d kv2(5, 10, 15);
		Vec<double> v2(5, 10, 15);

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				kv2.X() = kv2.X()++;
				kv2.Y() = kv2.Y()++;
				kv2.Z() = kv2.Z()++;
			}

			{
				START_TEST(participants[1]);
				v2.X() = v2.X()++;
				v2.Y() = v2.Y()++;
				v2.Z() = v2.Z()++;
			}
		}
	}

	void VectorSpeedTest::Vec4d()
	{
		const std::vector<std::string_view> participants = { "kmaths::Vector4d", "Tightly Packed Vector" };
		SetUpParticipants(participants);

		Vec<double> v2(5, 10, 15, 20);
		Vector4d kv2(5, 10, 15, 20);

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				kv2.X() = kv2.X()++;
				kv2.Y() = kv2.Y()++;
				kv2.Z() = kv2.Z()++;
				kv2.W() = kv2.W()++;
			}

			{
				START_TEST(participants[1]);
				v2.X() = v2.X()++;
				v2.Y() = v2.Y()++;
				v2.Z() = v2.Z()++;
				v2.W() = v2.W()++;
			}
		}
	}

	void VectorSpeedTest::Vec2bit()
	{
		const std::vector<std::string_view> participants = { "kmaths::Vector2<Big_Int_Type>", "Tightly Packed Vector" };
		SetUpParticipants(participants);

		Vec<Big_Int_Type> v2(5, 10);
		Vector2<Big_Int_Type> kv2(5, 10);

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				kv2.X() = kv2.X()++;
				kv2.Y() = kv2.Y()++;
			}

			{
				START_TEST(participants[1]);
				v2.X() = v2.X()++;
				v2.Y() = v2.Y()++;
			}
		}
	}

	void VectorSpeedTest::Vec3bit()
	{
		const std::vector<std::string_view> participants = { "kmaths::Vector3<Big_Int_Type>", "Tightly Packed Vector" };
		SetUpParticipants(participants);

		Vec<Big_Int_Type> v2(5, 10, 15);
		Vector3<Big_Int_Type> kv2(5, 10, 15);

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				kv2.X() = kv2.X()++;
				kv2.Y() = kv2.Y()++;
				kv2.Z() = kv2.Z()++;
			}

			{
				START_TEST(participants[1]);
				v2.X() = v2.X()++;
				v2.Y() = v2.Y()++;
				v2.Z() = v2.Z()++;
				v2.W() = v2.W()++;
			}
		}
	}

	void VectorSpeedTest::Vec4bit()
	{
		const std::vector<std::string_view> participants = { "kmaths::Vector4<Big_Int_Type>", "Tightly Packed Vector" };
		SetUpParticipants(participants);

		Vec<Big_Int_Type> v2(5, 10, 15, 20);
		Vector4<Big_Int_Type> kv2(5, 10, 15, 20);

		for (auto i = 0; i < maxIter; ++i)
		{
			{
				START_TEST(participants[0]);
				kv2.X() = kv2.X()++;
				kv2.Y() = kv2.Y()++;
				kv2.Z() = kv2.Z()++;
				kv2.W() = kv2.W()++;
			}

			{
				START_TEST(participants[1]);
				v2.X() = v2.X()++;
				v2.Y() = v2.Y()++;
				v2.Z() = v2.Z()++;
				v2.W() = v2.W()++;
			}
		}
	}
}
#endif