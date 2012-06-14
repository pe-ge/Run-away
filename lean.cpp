#include "processObjects.h"

void processLean(int id) {
	int lean = objectX[id] + 220;
	lean -= pos.x;
	lean *= (3.0/11.0);
	lean -= 5;

	programLean(objectX[id], objectY[id] + lean, objectX[id] + 220, objectY[id] + lean + 50);

	rObj.x = objectX[id] - camx;
	rObj.y = objectY[id] - camy;

	if (rObj.x > -300 && rObj.x < 640)
		SDL_BlitSurface(stairs, NULL, myscreen, &rObj);
}