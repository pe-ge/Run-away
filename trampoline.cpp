#include "processObjects.h"

void processTrampoline(int id) {
	if (programPlatform(objectX[id], objectY[id], objectX[id] + 75, objectY[id] + 22)) {
		if (effectsStatus)
			Mix_PlayChannel(1, jump, 0);
		giveJumpPulse(TRAMPOLINE_POWER);
	}

	rObj.x = objectX[id] - camx;
	rObj.y = objectY[id] - camy;

	if (rObj.x > -300 && rObj.x < 640)
		SDL_BlitSurface(trampoline, NULL, myscreen, &rObj);
}