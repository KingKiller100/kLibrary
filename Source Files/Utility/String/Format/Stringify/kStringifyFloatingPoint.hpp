#pragma once


#include "kStringifyHelper.hpp"
#include "kStringifyInteger.hpp"

#include "../../kStringConverter.hpp"
#include "../../../Debug/Exceptions/StringExceptions.hpp"

#include "../../../../TypeTraits/FloatTraits.hpp"
#include "../../../../HelperMacros.hpp"
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
			size_t dpShifts;
			bool isNeg;
		};

		template <class T>
		USE_RESULT constexpr kmaths::constants::Accuracy_t GetSignificantFigures(T val, size_t precision)
		{
			using namespace kmaths;
			using namespace kmaths::secret::impl;
			using namespace kmaths::constants;

			const auto accuracy = kmaths::secret::impl::PowerOfImpl<Accuracy_t>(CAST(Accuracy_t, 10), precision);
			const auto accuracyInverse = constants::OneOver<Accuracy_t>(accuracy);
			const auto dpShifts = constants::ZeroPointFive<Accuracy_t>() * accuracyInverse;

			const auto valuePlusDpsByAcc = (CAST(constants::Accuracy_t, val) + dpShifts) * accuracy;
			const auto sigFigs = Floor(valuePlusDpsByAcc);
			return static_cast<Accuracy_t>(sigFigs);
		}

		template<typename T>
		USE_RESULT constexpr size_t GetDpShifts(T decimals)
		{
			size_t count = 0;
			if (!kmaths::constants::ApproximatelyZero<T>(decimals))
			{
				size_t magnitude = 1;
				auto val = decimals * magnitude;
				while (val < 1)
				{
					++count;
					magnitude *= 10;
					val = decimals * magnitude;
				}
			}
			return count;
		}

		USE_RESULT inline size_t TrimZeros(size_t num)
		{
			while (num != 0 && kmaths::Modulus<size_t>(num, 10) == 0)
			{
				num /= static_cast<size_t>(10);
			}
			return num;
		}

		template<typename T>
		USE_RESULT constexpr Figures GetFigures(T val, size_t decimalPlaces)
		{
			const auto isNeg = kmaths::IsNegative(val);
			val = kmaths::Abs(val);
			const auto justIntegers = static_cast<size_t>(kmaths::Floor(val));
			const auto justDecimals = val - justIntegers;
			const auto dpShifts = GetDpShifts(justDecimals);
			const auto rawDecimalsToUse = GetSignificantFigures(justDecimals, decimalPlaces);
			const auto decimalsToUse = static_cast<size_t>(rawDecimalsToUse);
			return { justIntegers, decimalsToUse, dpShifts, isNeg };
		}

		template<class Char_t, typename T, typename = std::enable_if_t<
			std::is_floating_point_v<T>
			|| type_trait::Is_CharType_V<Char_t>>
			>
			const Char_t* FixedNotation(T val, size_t decimalPlaces, Figures& figs)
		{
			using namespace type_trait;

			using Traits = FloatTraits<T>;
			
			size_t num = 0;
			int count = 1;
			
			for (auto bit : Traits::Mask)
			{
				const auto uintBits = FloatTraits<T>::BitCast(val);
				const auto presion = FloatTraits<T>::Parts(val);
				const auto mant = presion.parts.mantissa;
				const bool hit = mant & bit;
				if (hit)
				{
					num += 1 * std::pow(T(2), -count);
				}
				++count;
			}

			Char_t buff[g_MaxFloatDigits<T>]{ g_NullTerminator<Char_t> };
			Char_t* const end = std::end(buff) - 1;
			Char_t* current = end;

			if (decimalPlaces > 0)
			{
				current = UintToStr(current, figs.decimals, 10);
				const auto padding = (end - current) + (figs.dpShifts - 1);
				PrependPadding(current, padding, Char_t('0'));

				*(--current) = Char_t('.');
			}

			current = UintToStr(current, figs.integers, 10);

			if (figs.isNeg)
				PrependMinusSign(current);

			auto cstr = CreateNewCString(current);
			return std::move(cstr);
		}

		template<class Char_t, typename T, typename = std::enable_if_t<
			std::is_floating_point_v<T>
			|| type_trait::Is_CharType_V<Char_t>>
			>
			const Char_t* ScientificNotation(T val, size_t decimalPlaces, Figures& figs)
		{
			using namespace type_trait;
			using String_t = std::basic_string<Char_t>;

			if (figs.integers != 0 && figs.integers < 10)
				return FixedNotation<Char_t>(val, figs.dpShifts + kmaths::CountIntegerDigits(figs.decimals), figs);

			constexpr auto defaultDps = std::numeric_limits<size_t>::digits10;
			if (kmaths::CountIntegerDigits(figs.decimals) < decimalPlaces
				&& decimalPlaces < defaultDps)
			{
				figs = GetFigures(val, defaultDps);
			}

			figs.decimals = TrimZeros(figs.decimals);

			Char_t buff[g_MaxFloatDigits<T>]{ g_NullTerminator<Char_t> };
			Char_t* const end = std::end(buff) - 1;
			Char_t* current = end;

			size_t totalShifts = 0;

			if (figs.integers > 0)
			{
				if (figs.integers > 9)
				{
					totalShifts += kmaths::CountIntegerDigits(figs.integers);
					--totalShifts;
				}
				figs.integers = TrimZeros(figs.integers);
			}

			if (figs.decimals > 0)
			{
				if (figs.decimals > 9
					|| (figs.integers == 0 && figs.decimals > 0))
					totalShifts += figs.dpShifts;
			}

			if (totalShifts > 0)
			{
				current = UintToStr(current, totalShifts, 10);
				if (val < 1)
					PrependMinusSign(current);
				*(--current) = g_ScientificFloatToken<Char_t>;
			}

			if (figs.decimals > 0)
			{
				current = UintToStr(current, figs.decimals, 10);
			}

			if (figs.integers > 0)
			{
				if (figs.dpShifts > 1)
				{
					auto dpShifts = figs.dpShifts;
					while (dpShifts-- > 0)
					{
						*(--current) = s_DefaultPlaceHolder<Char_t>;
					}
				}
				current = UintToStr(current, figs.integers, 10);
			}

			const auto ePos = Find_First_Of(current, g_ScientificFloatToken<Char_t>);
			if (ePos > 1)
			{
				*(--current) = Char_t('.');
				kmaths::Swap(current[0], current[1]);
			}

			if (figs.isNeg)
				PrependMinusSign(current);

			return CreateNewCString(current);
		}

		template<class Char_t, typename T, typename = std::enable_if_t<
			std::is_floating_point_v<T>
			|| type_trait::Is_CharType_V<Char_t>>
			>
			const Char_t* GeneralNotation(T val, size_t decimalPlaces, Figures& figs)
		{
			const auto cstr = figs.dpShifts > 6 || decimalPlaces > 6
				? ScientificNotation<Char_t>(val, decimalPlaces, figs)
				: FixedNotation<Char_t>(val, decimalPlaces, figs);
			return std::move(cstr);
		}

		// Big Endian
		template<class Char_t, typename Floating_t, typename Unsigned_t, typename = std::enable_if_t<
			std::is_unsigned_v<Unsigned_t>
			|| type_trait::Is_CharType_V<Char_t>
			>>
			const Char_t* BinaryNotation(Unsigned_t val)
		{
			constexpr auto dotIndex = type_trait::FloatTraits<Floating_t>::DotIndex;

			Char_t buff[g_MaxBits<Unsigned_t> +1]{ type_trait::g_NullTerminator<Char_t> };
			Char_t* const end = std::end(buff) - 1;
			Char_t* current = end;

			while (val > 0)
			{
				const auto binVal = val % 2;
				const Char_t digit = static_cast<Char_t>('0' + binVal);
				*(--current) = digit;
				val >>= 1;

				if (std::distance(current, end) == dotIndex)
					*(--current) = Char_t('.');
			}

			if (const auto distance = std::distance(current, end);
				distance < dotIndex)
			{
				PrependPadding(current, dotIndex, Char_t('0'));
				*(--current) = Char_t('.');
				*(--current) = Char_t('0');
			}

			if (!kmaths::IsNegative(val))
				*current = Char_t('0');

			auto cstr = CreateNewCString(current);
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

		decimalPlaces = kmaths::Min<size_t>(decimalPlaces, 25);

		if (std::isnan(val)) return Convert<Char_t>("nan");
		if (std::isinf(val)) return Convert<Char_t>("inf");

		Figures figs = GetFigures(val, decimalPlaces);

		switch (fmt.ToEnum()) {
		case FloatingPointFormat::FIX: return FixedNotation<Char_t>(val, decimalPlaces, figs);
		case FloatingPointFormat::SCI: return ScientificNotation<Char_t>(val, decimalPlaces, figs);
		case FloatingPointFormat::GEN: return GeneralNotation<Char_t>(val, decimalPlaces, figs);
		case FloatingPointFormat::HEX: return StringIntegralHex<Char_t>(type_trait::FloatTraits<T>::BitCast(val));
		case FloatingPointFormat::BIN: return BinaryNotation<Char_t, T>(type_trait::FloatTraits<T>::BitCast(val));
		default:
			throw kDebug::FormatError("Unknown floating point notation: " + ToWriter(fmt.ToString()));
		}
	}

}
