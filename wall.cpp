#include "processObjects.h"

void processWall(int id) {
	programSolid(objectX[id], objectY[id], objectX[id] + objXL[8], objectY[id] + objYL[8]);

	rObj.x = objectX[id] - camx;
	rObj.y = objectY[id] - camy;

	if (rObj.x > -300 && rObj.x < 640)
		SDL_BlitSurface(wall, NULL, myscreen, &rObj);
}