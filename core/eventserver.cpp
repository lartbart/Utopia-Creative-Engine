#include "eventserver.h"

EventServer::EventServer() {

}

EventServer& EventServer::getInstance() {
	static EventServer instance;
	return instance;
}

Event EventServer::processEvents(const std::vector<int>& events_array, Object* object) {
	if (VisualObject* visual_object = dynamic_cast<VisualObject*>(object)) {
		if (events_array[0] > visual_object->x && events_array[0] < (visual_object->x + visual_object->image.width) && events_array[1] > visual_object->y && events_array[1] < (visual_object->y + visual_object->image.height) && events_array[2] == 1) {
			return {EventType::Pressed, visual_object};
		}		
	}
	return {EventType::None, object}; 
}
