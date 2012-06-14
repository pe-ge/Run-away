#include "processObjects.h"

void processPickedBall(int id) {
	keystate = SDL_GetKeyState(NULL);

	objectX[id] = lpx;
	objectY[id] = lpy - 37;

	rObj.x = objectX[id] - camx;
	rObj.y = objectY[id] - camy;

	if (keystate[SDLK_SPACE]) {
		if (!spaceWasDown) {
			// ball thrown
			spaceWasDown = true;
			objectT[id] = 156;
			objData2[id] = 0;
			if (isFacingRight)
				objData1[id] = BOWLINGBALL_SPEED;
			else
				objData1[id] = -BOWLINGBALL_SPEED;
		}
	} else
		spaceWasDown = false;

	if (rObj.x > -300 && rObj.x < 640)
		SDL_BlitSurface(ball, NULL, myscreen, &rObj);
}