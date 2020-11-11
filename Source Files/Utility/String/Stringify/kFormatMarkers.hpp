#pragma once

#include <cstdint>
#include <deque>
#include <string>

namespace klib::kString::secret::helper
{
	struct FormatMarker
	{
		using IndexType = std::uint16_t;
		using PositionType = size_t;
		using NameType = std::string;
		
		IndexType objIndex;
		PositionType position;
		NameType type;

		FormatMarker(IndexType objIndex, PositionType position, NameType type)
			: objIndex(objIndex), position(position), type(type)
		{}
	};

	using FormatMarkerQueue = std::deque<FormatMarker>;
}
