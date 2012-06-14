#include "processObjects.h"

void processFlippingPlatform(int id) {
	if (programPlatform(objectX[id], objectY[id], objectX[id] + 219, objectY[id] + 37)) {
		objData1[id]++;
		if (objData1[id] > FLIP_DELAY) {
			objData1[id] = 0;
			objectT[id] = 109;
		}
	}

	rObj.x = objectX[id] - camx;
	rObj.y = objectY[id] - camy;

	if (rObj.x > -300 && rObj.x < 640)
		SDL_BlitSurface(fliptf, NULL, myscreen, &rObj);
}

void processFlippingPlatform2(int id) {
	if (objData1[id] > FLIP_ANIM + 5) {
		objData1[id]++;
		if (objData1[id] > FLIP_ANIM + FLIP_STAY + 5) {
			objData1[id] = 0;
			objectT[id] = 9;
		}
	} else {
		objData1[id]++;
		if (objData1[id] > FLIP_ANIM) {
			objData1[id] = 0;
			objectT[id] = 209;
		}
	}

	rObj.x = objectX[id] - camx;
	rObj.y = objectY[id] - camy;

	if (rObj.x > -300 && rObj.x < 640)
		SDL_BlitSurface(fliptf2, NULL, myscreen, &rObj);
}

void processFlippingPlatform3(int id) {
	objData1[id]++;
	if (objData1[id] > FLIP_ANIM) {
		objData1[id] = FLIP_ANIM + 10;
		objectT[id] = 109;
	}

	rObj.x = objectX[id] - camx;
	rObj.y = objectY[id] - camy;

	if (rObj.x > -300 && rObj.x < 640)
		SDL_BlitSurface(fliptf3, NULL, myscreen, &rObj);
}