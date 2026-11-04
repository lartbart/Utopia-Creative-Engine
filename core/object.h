#pragma once
#include <string>
#include <vector>
#include <functional>

class Object {
	public:
		Object() = default;
		virtual ~Object() = default;
		std::string name = "";
		std::vector<Object*> children = {};
		int event_type = 0;
		std::function<void()> event_function;
};
