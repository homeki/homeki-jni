#include "tellstickeventqueue.h"

TellstickEventQueue::TellstickEventQueue() {
	sem = new sem_t();
	mutex = new sem_t();
	sem_init(sem, 0, 0);
	sem_init(mutex, 0, 1);
}

TellstickEventQueue::~TellstickEventQueue() {
	sem_destroy(sem);
	sem_destroy(mutex);
}

void TellstickEventQueue::addEvent(std::string event) {
	sem_wait(mutex);
	eventQueue.push(event);
	sem_post(mutex);
	sem_post(sem);
}

std::string TellstickEventQueue::getEvent() {
	sem_wait(sem);
	sem_wait(mutex);
	std::string event = eventQueue.front();
	eventQueue.pop();
	sem_post(mutex);
	return event;
}
