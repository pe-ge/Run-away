#include "processObjects.h"

void processMovingPlatform(int id) {
	// pohybujuca sa platforma zlava doprava
	bool collides = programPlatform(objectX[id], objectY[id], objectX[id] + 219, objectY[id] + 37);
	if (collides)
		fallSpeed = 0;
	if (cycles < (TOTALCYCLES / 2)) {
		int distX,distY;
		distX = objectX[id + 1] - objectX[id + 2];
		distY = objectY[id + 1] - objectY[id + 2];
		objectX[id] = objectX[id + 1] - (objectX[id + 1] - objectX[id + 2]) * (cycles / (TOTALCYCLES / 2));
		objectY[id] = objectY[id + 1] - (objectY[id + 1] - objectY[id + 2]) * (cycles / (TOTALCYCLES / 2));
	} else {
		int distX, distY;
		distX = objectX[id + 2] - objectX[id + 1];
		distY = objectY[id + 2] - objectY[id + 1];
		objectX[id] = objectX[id + 2] + (objectX[id + 1] - objectX[id + 2]) * ((cycles - (TOTALCYCLES / 2)) / (TOTALCYCLES / 2));
		objectY[id] = objectY[id + 2] + (objectY[id + 1] - objectY[id + 2]) * ((cycles - (TOTALCYCLES / 2)) / (TOTALCYCLES / 2));
	}

	// pohni hracom
	if (collides && !jumping) {
		pos.x += objectX[id] - objData1[id];
		pos.y = objectY[id] - playerIdleLeft->h + 1;
	}

	SDL_BlitSurface(platform, NULL, myscreen,&rObj); 

	// zapamataj stare suradnice
	objData1[id] = objectX[id];
}