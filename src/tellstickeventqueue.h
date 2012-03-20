#ifndef TELLSTICKEVENTQUEUE_H_
#define TELLSTICKEVENTQUEUE_H_

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <queue>
#include <string>

class TellstickEventQueue {
public:
	TellstickEventQueue();
	virtual ~TellstickEventQueue();

	void addEvent(std::string event);
	std::string getEvent();

private:
	boost::condition_variable cond;
	boost::mutex mut;
	std::queue<std::string> eventQueue;
};

#endif
