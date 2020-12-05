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
#include "../../../../Maths/kFloatBreakdown.hpp"

#include <utility>


namespace klib::kString::stringify
{
	namespace secret::impl
	{
		using namespace kmaths;

		constexpr auto g_MaxPrecision = 25;

		template<class Char_t, typename T, typename = std::enable_if_t<
			std::is_floating_point_v<T>
			|| type_trait::Is_CharType_V<Char_t>>
			>
			const Char_t* FixedNotation(T val, size_t decimalPlaces, FloatBreakdown<T>& breakdown)
		{
			using namespace type_trait;

			Char_t buff[g_MaxFloatDigits<T>]{ g_NullTerminator<Char_t> };
			Char_t* const end = std::end(buff) - 1;
			Char_t* current = end;

			if (decimalPlaces > 0)
			{
				size_t decimals = static_cast<size_t>(breakdown.decimals * std::pow(10, decimalPlaces + 1));
				decimals += 5;
				Demote(decimals);
				current = UintToStr(current, decimals);
				const auto desiredCharacterCount = (end - current) + (breakdown.dpShifts - 1);
				PrependPadding(current, desiredCharacterCount, Char_t('0'));
				*(--current) = Char_t('.');
			}

			current = UintToStr(current, breakdown.integers, 10);

			if (breakdown.isNeg)
				PrependMinusSign(current);

			return CreateNewCString(current);
		}

		template<class Char_t, typename T, typename = std::enable_if_t<
			std::is_floating_point_v<T>
			|| type_trait::Is_CharType_V<Char_t>>
			>
			const Char_t* ScientificNotation(T val, size_t sigFigs, FloatBreakdown<T>& breakdown)
		{
			using namespace type_trait;

			if (breakdown.integers == 0 && breakdown.dpShifts == 0)
				return Convert<Char_t>("0");

			Char_t buff[g_MaxFloatDigits<T>]{ g_NullTerminator<Char_t> };
			Char_t* const end = std::end(buff) - 1;
			Char_t* current = end;

			const auto isZeroInt = breakdown.integers == 0;
			const auto direction = kmaths::IsDecimal(val)
				? Char_t('-')
				: Char_t('+');

			const auto intDigits = kmaths::CountIntegerDigits(breakdown.integers);
			const auto sizeMet = !isZeroInt && intDigits >= sigFigs;
			auto exponent = intDigits - 1;

			if (sizeMet) // Only need integer significant figures
			{
				current = UintToStr(current, exponent);
				if (exponent < 10) current = UintToStr(current, 0);
				*(--current) = direction;
				*(--current) = g_ScientificFloatToken<Char_t>;
				if (intDigits > sigFigs) // Integers size > significant figures
				{
					const auto limit = static_cast<size_t>(std::pow(10, sigFigs + 1));
					while (breakdown.integers >= limit)
					{
						kmaths::Demote(breakdown.integers);
					}
					breakdown.integers += 5;
					kmaths::Demote(breakdown.integers);
				}
				else // Integer == significant figures
				{
					if (breakdown.decimals >= kmaths::ZeroPointFive<T>())
					{
						++breakdown.integers;
					}
				}
			}
			else if (intDigits + breakdown.dpShifts == sigFigs)
			{
				size_t decimals = static_cast<size_t>(breakdown.decimals * std::pow(10, breakdown.dpShifts + 1));
				decimals += 5;
				Demote(decimals);
				
				current = UintToStr(current, exponent);
				if (exponent < 10) current = UintToStr(current, 0);
				*(--current) = direction;
				*(--current) = g_ScientificFloatToken<Char_t>;
				
				if (decimals != 0)
					current = UintToStr(current, decimals);
			}
			else // Need integer and decimals significant figures
			{
				auto remaining = isZeroInt ? sigFigs : (sigFigs - (intDigits + breakdown.dpShifts - 1));
				const auto mag = std::pow(10, remaining);
				size_t power = 1;
				size_t shifts = 1;
				size_t decimals = 0;

				while (decimals < mag)
				{
					decimals = static_cast<size_t>(breakdown.decimals * std::pow(10, power++));

					if (decimals == 0)
						++shifts;
				}

				decimals += 5;
				Demote(decimals);
				
				exponent += isZeroInt ? shifts : breakdown.integers > 9 ? shifts - 1 : 0;
				current = UintToStr(current, exponent);
				if (exponent < 10) current = UintToStr(current, 0);
				*(--current) = direction;
				*(--current) = g_ScientificFloatToken<Char_t>;

				if (decimals != 0)
					current = UintToStr(current, decimals);

				while (!isZeroInt && shifts-- > 1)
				{
					*(--current) = Char_t('0');
				}
			}

			if (!isZeroInt)
				current = UintToStr(current, breakdown.integers);

			if (current[1] != g_ScientificFloatToken<Char_t>)
			{
				*(--current) = Char_t('.');
				kmaths::Swap(current[0], current[1]);
			}

			if (breakdown.isNeg)
				PrependMinusSign(current);

			return CreateNewCString(current);
		}

		template<class Char_t, typename T, typename = std::enable_if_t<
			std::is_floating_point_v<T>
			|| type_trait::Is_CharType_V<Char_t>>
			>
			const Char_t* GeneralNotation(T val, size_t sigFigs, FloatBreakdown<T>& figs)
		{
			return figs.dpShifts > 5 || sigFigs > 5
				? ScientificNotation<Char_t>(val, sigFigs, figs)
				: FixedNotation<Char_t>(val, sigFigs, figs);
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
	ENUM_CLASS(FloatFormat, std::uint8_t,
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
		const Char_t* StringFloatingPoint(T val, size_t figures = s_NoSpecifier
			, FloatFormat fmt = FloatFormat::FIX)
	{
		using namespace secret::impl;
		using Traits = type_trait::FloatTraits<T>;

		if (std::isnan(val)) return Convert<Char_t>("nan");
		if (std::isinf(val)) return Convert<Char_t>("inf");

		if (figures == s_NoSpecifier)
			figures = 1;
		else
			figures = kmaths::Min<size_t>(figures, g_MaxPrecision);

		if (fmt.MaskCmp(FloatFormat::GEN))
		{
			FloatBreakdown breakdown = GetFigures(val, figures);

			switch (fmt.ToEnum()) {
			case FloatFormat::FIX: return FixedNotation<Char_t>(val, figures, breakdown);
			case FloatFormat::SCI: return ScientificNotation<Char_t>(val, figures + 1, breakdown);
			case FloatFormat::GEN: return GeneralNotation<Char_t>(val, figures, breakdown);
			default: throw kDebug::FormatError("Unknown floating point notation: " + ToWriter(fmt.ToString()));
			}
		}

		return fmt.Compare(FloatFormat::HEX
			, StringIntegralHex<Char_t>(Traits::UintBitCast(val))
			, BinaryNotation<Char_t, T>(Traits::UintBitCast(val))
		);
	}

}

