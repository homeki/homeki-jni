#include "tellstickeventqueue.h"

TellstickEventQueue::TellstickEventQueue() {

}

TellstickEventQueue::~TellstickEventQueue() {

}

void TellstickEventQueue::addEvent(std::string event) {
	boost::lock_guard<boost::mutex> lock(mut);
	eventQueue.push(event);
	cond.notify_all();
}

std::string TellstickEventQueue::getEvent() {
	boost::unique_lock<boost::mutex> lock(mut);

    while(eventQueue.empty())
        cond.wait(lock);

	std::string event = eventQueue.front();
	eventQueue.pop();

	return event;
}
