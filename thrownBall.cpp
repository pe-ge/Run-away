#include "processObjects.h"

void processThrownBall(int id) {
	keystate = SDL_GetKeyState(NULL);

	objectX[id] += objData1[id];
	objectY[id] += objData2[id];

	if (objData2[id] < BOWLINGBALL_VSPEED)
		objData2[id]++;

	int i = 1;
	rObj.x = objectX[id] - camx;
	rObj.y = objectY[id] - camy;

	while (i < numberOfObjects) {
		if (i != id) {
			int current = objectT[i];
			if (typeDef[current] == TYPE_PLATFORM) {
				int tmp = doComplexPlatform(objectX[id], objectY[id], objectX[id] + 37,objectY[id] + 38, objectX[i], objectY[i], objectX[i] + objXL[current], objectY[i] + objYL[current]);
				if (tmp != objectY[id])
					objData2[id] = 0;

				objectY[id] = tmp;
			}

			if (typeDef[current] == TYPE_SOLID) {

				rObj = doComplexSolid(objectX[id], objectY[id], objData3[id], objectX[id] + 37,objectY[id] + 38, objectX[i], objectY[i], objectX[i] + objXL[current], objectY[i] + objYL[current]);
				if (rObj.y != objectY[id])
					objData2[id] = 0;

				objectX[id] = rObj.x;
				objectY[id] = rObj.y;
			}

			if (typeDef[current] == TYPE_ENEMY) {
				if (objectX[id] > objectX[i] && objectX[id] < objectX[i] + objXL[i])
					if (objectY[id] > objectY[i] && objectY[id] < objectY[i] + objYL[i])
						killEnemy(i);
			}
		}
		i++;
	}

	rObj.x = objectX[id] - camx;
	rObj.y = objectY[id] - camy;

	if (rObj.x > -300 && rObj.x < 640)
		SDL_BlitSurface(ball, NULL, myscreen, &rObj);

	if (objData2[id] < 2 && objData3[id] == objectX[id])
		objectT[id] = 6;

	objData3[id] = objectX[id];
}