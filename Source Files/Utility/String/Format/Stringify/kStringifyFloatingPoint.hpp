#pragma once


#include "kStringifyHelper.hpp"
#include "kStringifyInteger.hpp"

#include "../../kStringConverter.hpp"

#include "../../../Debug/Exceptions/StringExceptions.hpp"
#include "../../../../Maths/Length_Type.hpp"
#include "../../../../Maths/kMathsFundamentals.hpp"
#include "../../../../Maths/kModulus.hpp"
#include "../../../../Maths/kRound.hpp"

#include <utility>

namespace klib::kString::stringify
{
	namespace secret::impl
	{
		struct Figures
		{
			size_t integers;
			size_t decimals;
			bool isNeg;
		};

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

		template<typename T>
		USE_RESULT constexpr Figures GetFigures(T val, size_t decimalPlaces)
		{
			const auto isNeg = kmaths::IsNegative(val);
			val = kmaths::Abs(val);
			const auto justIntegers = static_cast<size_t>(kmaths::Floor(val));
			const auto justDecimals = val - justIntegers;
			const auto decimalsToUse = static_cast<size_t>(GetSignificantFigures(justDecimals, decimalPlaces));
			return { justIntegers, decimalsToUse, isNeg };
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

			const Figures figures = GetFigures(val, decimalPlaces);

			Char_t buff[g_MaxFloatDigits<T>]{ g_NullTerminator<Char_t> };
			Char_t* const end = std::end(buff) - 1;
			Char_t* current = end;

			if (decimalPlaces > 0)
			{
				current = UintToStr(current, figures.decimals);
				PrependPadding(current, decimalPlaces, Char_t('0'));
				*(--current) = Char_t('.');
			}

			current = UintToStr(current, figures.integers);

			if (figures.isNeg)
				*(--current) = Char_t('-');

			auto cstr = CreateNewCString(current);
			return std::move(cstr);
		}

		inline USE_RESULT size_t RemoveTrailingZeros(size_t num)
		{
			while (num != 0 && kmaths::Modulus<size_t>(num, 10) == 0)
			{
				num /= static_cast<size_t>(10);
			}
			return num;
		}

		template<class Char_t, typename T, typename = std::enable_if_t<
			std::is_floating_point_v<T>
			|| type_trait::Is_CharType_V<Char_t>>
			>
			const Char_t* ScientificNotation(T val, size_t decimalPlaces)
		{
			using namespace type_trait;

			if (decimalPlaces == s_NoSpecifier)
				decimalPlaces = 6;


			size_t totalDigits = 0;

			Figures figs = GetFigures(val, decimalPlaces);
			if (figs.integers > 0)
			{
				if (figs.integers > 9)
				{
					totalDigits += kmaths::CountIntegerDigits(figs.integers);
					--totalDigits;
				}
				figs.integers = RemoveTrailingZeros(figs.integers);
			}
			if (figs.decimals > 0)
			{
				figs.decimals = RemoveTrailingZeros(figs.decimals);
				if (figs.decimals > 9
					|| (figs.integers == 0 && figs.decimals > 0))
					totalDigits += kmaths::CountIntegerDigits(figs.decimals);
			}


			Char_t buff[g_MaxFloatDigits<T>]{ g_NullTerminator<Char_t> };
			Char_t* const end = std::end(buff) - 1;
			Char_t* current = end;

			current = UintToStr(current, totalDigits);
			if (val < 1)
				*(--current) = Char_t('-');
			*(--current) = Char_t('e');
			if (figs.decimals > 0)
				current = UintToStr(current, figs.decimals);
			if (figs.integers > 0)
			{
				current = UintToStr(current, figs.integers);

				if (figs.integers > 9 || figs.decimals > 0)
				{
					*(--current) = Char_t('.');
					kmaths::Swap(current[0], current[1]);
				}
			}

			if (figs.isNeg)
				*(--current) = Char_t('-');

			auto cstr = CreateNewCString(current);
			return std::move(cstr);
		}

		template<class Char_t, typename T, typename = std::enable_if_t<
			std::is_floating_point_v<T>
			|| type_trait::Is_CharType_V<Char_t>>
			>
			const Char_t* GeneralNotation(T val, size_t decimalPlaces)
		{
			const auto cstr = decimalPlaces > 6
				? ScientificNotation<Char_t>(val, decimalPlaces)
				: FixedNotation<Char_t>(val, decimalPlaces);
			return std::move(cstr);
		}
	}

	/// <summary>
	/// Floating point formatting for string
	/// </summary>
	/// <param name="FIX">Fixed notation</param>
	/// <param name="SCI">Scientific notation</param>
	/// <param name="GEN">General notation (either scientific or fixed)</param>
	/// <param name="HEX">Hexadecimal format</param>
	/// <param name="BIN">Binary format</param>
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
		const Char_t* StringFloatingPoint(T val, size_t decimalPlaces = s_NoSpecifier
			, FloatingPointFormat fmt = FloatingPointFormat::FIX)
	{
		using namespace secret::impl;
		using IntegralSizeMatch_t = std::conditional_t<sizeof(T) == 4, std::int32_t, std::int64_t>;

		switch (fmt.ToEnum()) {
		case FloatingPointFormat::FIX: return FixedNotation<Char_t>(val, decimalPlaces);
		case FloatingPointFormat::HEX: return StringIntegralBinary<Char_t>(*(IntegralSizeMatch_t*)&val, decimalPlaces);
		case FloatingPointFormat::BIN: return StringIntegralHex<Char_t>(*(IntegralSizeMatch_t*)&val, decimalPlaces);
		case FloatingPointFormat::SCI: return ScientificNotation<Char_t>(val, decimalPlaces);
		case FloatingPointFormat::GEN: return GeneralNotation<Char_t>(val, decimalPlaces);
		default:
			throw kDebug::FormatError("Unknown floating point notation: " + ToWriter(fmt.ToString()));
		}
	}

}
