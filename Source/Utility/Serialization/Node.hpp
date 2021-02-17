#pragma once

#include "../String/kToString.hpp"

#include <string>

#include <memory>

namespace kml
{
	struct NodeData;
	
	template<typename T>
	class Converter
	{
	public:
		static void Encode(NodeData* data, const T& input);
		static void Decode(NodeData* data, T& output);
	};
	
	struct NodeData
	{
		std::string rawData;
		std::string key;
		std::string value;

		template<typename T>
		void Get(T& out)
		{
			Converter<T>::Decode(*this, out);
		}

		template<typename T>
		void Get(T& out, const T& defaultValue)
		{
			try
			{
				Converter<T>::Decode(*this, out);
			}
			catch (...)
			{+
				out = defaultValue;
			}
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
