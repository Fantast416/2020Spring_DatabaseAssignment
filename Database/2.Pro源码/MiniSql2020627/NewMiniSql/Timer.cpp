
#include"Timer.h"
Timer& GetGlobalTimer() {
	static Timer timer;
	return timer;
}
