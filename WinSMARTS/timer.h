typedef void (*TIMER_CALLBACK)();

void setSigTimer(unsigned int ms, TIMER_CALLBACK cb);