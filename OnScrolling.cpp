#include "processObjects.h"

void processOnScrolling(int id) {
	if (pos.x > objectX[id])
		verticalScrolling = true;
}