#pragma once

#include "../String/kToString.hpp"

#include <string>

#include <memory>

namespace kml
{
	struct NodeData
	{
		std::string rawData;
		std::string key;
		std::string value;

		template<typename T>
		T Get()
		{
			return klib::kString::StrTo<T>(rawData);
		}

		template<typename T>
		T Get(const T& defaultValue)
		{
			return klib::kString::TryStrTo<T>(rawData, defaultValue);
		}
	};
	
	struct Node
	{
		std::unique_ptr<Node> parent;
		std::unique_ptr<Node> prev;
		std::unique_ptr<Node> next;
		NodeData data;

	};
}
