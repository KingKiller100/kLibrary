#pragma once


#include "kStringifyHelper.hpp"
#include "kStringifyInteger.hpp"

#include "../../kStringConverter.hpp"

#include "../../../Debug/Exceptions/StringExceptions.hpp"
#include "../../../../Maths/Length_Type.hpp"
#include "../../../../Maths/kMathsFundamentals.hpp"
#include "../../../../Maths/kRound.hpp"

#include <utility>

namespace klib::kString::stringify
{
	namespace secret::impl
	{
		template <class T>
		USE_RESULT constexpr T GetSignificantFigures(T val, size_t precision)
		{
			using namespace kmaths;
			using namespace kmaths::secret::impl;
			using namespace kmaths::constants;

			const auto accuracy = kmaths::secret::impl::PowerOfImpl<Accuracy_t>(CAST(Accuracy_t, 10), precision);
			const auto accuracyInverse = constants::OneOver<Accuracy_t>(accuracy);
			const auto dpShifts = constants::ZeroPointFive<Accuracy_t>() * accuracyInverse;

			const auto valuePlusDpsByAcc = (CAST(constants::Accuracy_t, val) + dpShifts) * accuracy;
			const auto sigFigs = Floor(valuePlusDpsByAcc);
			return static_cast<T>(sigFigs);
		}


		template<class Char_t, typename T, typename = std::enable_if_t<
			std::is_floating_point_v<T>
			|| type_trait::Is_CharType_V<Char_t>>
			>
			const Char_t* FixedNotation(T val, size_t decimalPlaces)
		{
			using namespace type_trait;

			if (decimalPlaces == s_NoSpecifier)
				decimalPlaces = 6;

			const auto isNeg = kmaths::IsNegative(val);

			val = kmaths::Abs(val);
			const auto justIntegers = static_cast<kmaths::BigInt_t>(kmaths::Floor(val));
			const auto justDecimals = val - (isNeg ? -justIntegers : justIntegers);
			auto decimalsToAppend = static_cast<size_t>(GetSignificantFigures(justDecimals, decimalPlaces));

			Char_t buff[s_MaxFloatDigits<T>]{ g_NullTerminator<Char_t> };
			Char_t* const end = std::end(buff) - 1;
			Char_t* current = end;

			if (decimalPlaces > 0)
			{
				current = UintToStr(current, decimalsToAppend);
				PrependPadding(current, decimalPlaces, Char_t('0'));
				*(--current) = Char_t('.');
			}

			current = UintToStr(current, justIntegers);

			if (isNeg)
				*(--current) = Char_t('-');

			auto cstr = CreateNewPointer(current);
			return std::move(cstr);
		}

	}

	/// <summary>
	/// Floating point formatting for string
	/// </summary>
	/// <param name="FIX">Fixed notation</param>
	/// <param name="SCI">Scientific notation</param>
	/// <param name="GEN">General notation (either scientific or fixed)</param>
	ENUM_CLASS(FloatingPointFormat, std::uint8_t,
		FIX = BIT_SHIFT(0),
		SCI = BIT_SHIFT(1),
		GEN = FIX | SCI,
		HEX = BIT_SHIFT(2),
		BIN = BIT_SHIFT(3)
	);

	template<class Char_t, typename T, typename = std::enable_if_t<
		std::is_floating_point_v<T>
		|| type_trait::Is_CharType_V<Char_t>>
		>
		const Char_t* StringFloatingPoint(T val, size_t precision = s_NoSpecifier
			, FloatingPointFormat fmt = FloatingPointFormat::FIX)
	{
		using namespace secret::impl;
		using IntegralSizeMatch_t = std::conditional_t<sizeof(T) == 4, std::int32_t, std::int64_t>;
		
		switch (fmt.ToEnum()) {
		case FloatingPointFormat::FIX: return FixedNotation<Char_t>(val, precision);
		case FloatingPointFormat::HEX: return StringIntegralBinary<Char_t>(*(IntegralSizeMatch_t*)&val, precision);
		case FloatingPointFormat::BIN: return StringIntegralHex<Char_t>(*(IntegralSizeMatch_t*)&val, precision);
		case FloatingPointFormat::SCI:
		case FloatingPointFormat::GEN:
		default:
			throw kDebug::FormatError("Unknown floating point notation: " + ToWriter(fmt.ToString()));
		}
	}

}
