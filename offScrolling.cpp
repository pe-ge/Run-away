#include "processObjects.h"

void processOffScrolling(int id) {
	if (pos.x > objectX[id])
		verticalScrolling = false;
}