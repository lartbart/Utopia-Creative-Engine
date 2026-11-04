#pragma once

#include <vector>
#include "object.h"
#include "visualobject.h"

enum EventType {
	None,
	Pressed
};

struct Event {
	EventType type;
	Object* object;
};

class EventServer {
		public:
			~EventServer() = default;
			static EventServer& getInstance();
			Event processEvents(const std::vector<int>& events_array, Object* object);
		private:
			EventServer();
};
