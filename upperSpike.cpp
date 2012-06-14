#include "processObjects.h"

void processUpperSpike(int id) {
	if (collides(objectX[id], objectY[id], 56, 64, pos.x, pos.y, playerIdleLeft->w, playerIdleLeft->h)) //zomreli sme
		pwr = 0;

	rObj.x = objectX[id] - camx;
	rObj.y = objectY[id] - camy;

	if (rObj.x > -300 && rObj.x < 640)
		SDL_BlitSurface(upperSpike, NULL, myscreen, &rObj);
}