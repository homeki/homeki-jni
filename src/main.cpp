#include <stdio.h>
#include <string>
#include "tellstickeventqueue.h"

int main() {
	TellstickEventQueue* evq = new TellstickEventQueue();

	evq->addEvent((std::string)"ett event!");
	std::string event = evq->getEvent();
	printf("%s\n", event.c_str());
	printf("into sem_wait()\n");
	evq->getEvent();

	return 0;
}
