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
		
		IndexType objIndex;
		PositionType position;

		FormatMarker(IndexType objIndex, PositionType position)
			: objIndex(objIndex), position(position)
		{}
	};

	using FormatMarkerQueue = std::deque<FormatMarker>;
}
