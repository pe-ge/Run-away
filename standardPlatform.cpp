#include "processObjects.h"

void processStandardPlatform(int id) {
	programPlatform(objectX[id], objectY[id], objectX[id] + 219, objectY[id] + 37);

	rObj.x = objectX[id] - camx;
	rObj.y = objectY[id] - camy;
	
	if (rObj.x > -300 && rObj.x < 640)
			SDL_BlitSurface(platform, NULL, myscreen, &rObj);
}