#ifndef TELLSTICKEVENTQUEUE_H_
#define TELLSTICKEVENTQUEUE_H_

#include <semaphore.h>
#include <queue>
#include <string>

class TellstickEventQueue {
public:
	TellstickEventQueue();
	virtual ~TellstickEventQueue();

	void addEvent(std::string event);
	std::string getEvent();

private:
	sem_t* sem;
	sem_t* mutex;
	std::queue<std::string> eventQueue;
};

#endif
