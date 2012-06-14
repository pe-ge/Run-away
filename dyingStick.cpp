#include "processObjects.h"

void processDyingStick(int id) {
	// spadne cez obrazovku dole

	objectY[id] += objData2[id];
	if (objData2[id] < 10)
		objData2[id]++;

	if (objData1[id] > 0) {
		objectX[id] += objData1[id];
		objData1[id]--;
	}

	rObj.x = objectX[id] - camx;
	rObj.y = objectY[id] - camy;

	if (rObj.x > -300 && rObj.x < 640)
		if (pos.x + 46 < objectX[id] + 50)
			SDL_BlitSurface(ironStick, NULL, myscreen, &rObj);
		else
			SDL_BlitSurface(ironStick2, NULL, myscreen, &rObj);
}