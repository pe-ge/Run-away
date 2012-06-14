#include "processObjects.h"

void processLowerSpike(int id) {
	if (collides(objectX[id], objectY[id], 56, 64, pos.x, pos.y, playerIdleLeft->w, playerIdleLeft->h))
        pwr = 0;

	rObj.x = objectX[id] - camx;
	rObj.y = objectY[id] - camy;

	if (rObj.x > -300 && rObj.x < 640)
		SDL_BlitSurface(lowerSpike, NULL, myscreen, &rObj); 
}