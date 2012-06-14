#include "processObjects.h"

void processBallOnGround(int id) {
	keystate = SDL_GetKeyState(NULL);

	int epx = pos.x + playerIdleLeft->w;
	int epy = pos.y + playerIdleLeft->h;

	if (collides(pos.x, pos.y, playerIdleLeft->w, playerIdleLeft->h, objectX[id], objectY[id], objXL[6], objYL[6]))
		if (keystate[SDLK_SPACE]) {
			if (!spaceWasDown) {
				//lopta bola hodena
				spaceWasDown = true;
				objectT[id] = 106;
			}
		} else
			spaceWasDown = false;


	if (rObj.x > -300 && rObj.x < 640)
		SDL_BlitSurface(ball, NULL, myscreen, &rObj);
}