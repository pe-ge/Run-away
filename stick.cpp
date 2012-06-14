#include "processObjects.h"

void processStick(int id) {
	if (objMode[id] < 0)
		objMode[id] = 0;

	int tmp = 0;
	if (objectX[id] - camx > 0 && objectX[id] - camx < 640) { //je ho vidiet na obrazovke?
		if (objMode[id] > 50) {
			if (pos.x < objectX[id])
				objectX[id] += 2;
			else
				objectX[id] -= 2;

			objMode[id]--;
		} else {
			tmp = 0;
			if (pos.x + 50 < objectX[id]) {
				tmp = 1;
				if (abs(pos.x - objectX[id]) < 200) {
					objectX[id] -= 8;
				}
			}

			if (pos.x > objectX[id] + objXL[1]) {
				tmp = 1;
				if (abs(pos.x - objectX[id]) < 200 ){
					objectX[id] += 8;
				}
			}

			if (tmp == 0)
				objMode[id] = 50 + IRONSTICK_ATTACK_DELAY;

			if (pos.x + 50 > objectX[id] - 10 && pos.x + 50 < objectX[id] + objXL[1] || pos.x < objectX[id] + objXL[1] && pos.x > objectX[id])
				if (pos.y + 50 > objectY[id] - 10 && pos.y + 50 < objectY[id] + objYL[1]) {
					if (animFrame[id] == 1)
					animFrame[id] = 2;

					if (objData4[id] > IRONSTICK_ATTACK_REPEATS) {
						objData4[id] = 1;
						pwr -= IRONSTICK_PLAYER_DAMAGE;
						objMode[id] = 50 + IRONSTICK_ATTACK_DELAY;
						if (effectsStatus)
							Mix_PlayChannel(3, punch, 0);
					} else {
						objMode[id] = 50 + IRONSTICK_ATTACK_DELAY;
						objData4[id]++;
					}
				}
		}
	}

	if (programPlatform(objectX[id], objectY[id], objectX[id] + 75,objectY[id] + 21)) {
		objectT[id] = 101; //zabil som ho
		giveJumpPulse(DEF_JPOWER + 1);
		if (effectsStatus)
			Mix_PlayChannel(1, jump, 0);
	}
	objectY[id] += objData2[id];
	if (objData2[id] < 10)
		objData2[id]++;
	
	int platform = onPlatform[id];
	onPlatform[id] = 0;
	int i = 1;
	while (i < numberOfObjects) {
		if (i != id) {
			int current = objectT[i];
			if (typeDef[current] == TYPE_PLATFORM) {
				if (platform == i)
					if (current == 13)
						if (objData3[i] == 1)
							objectY[id] += PLATFORMS_MOVESPEED + 1;

				int tmp = doComplexPlatform(objectX[id], objectY[id], objectX[id] + objXL[1], objectY[id] + objYL[1], objectX[i], objectY[i], objectX[i] + objXL[current], objectY[i] + objYL[current]);

				if (tmp != objectY[id]) {
					objData2[id] = 0;
					onPlatform[id] = i;
				}
				objectY[id] = tmp;
			}

			//kolizia nepriatelov
			if (typeDef[current] == TYPE_SOLID || typeDef[current] == TYPE_ENEMY) {
				current = objectT[i];

				rObj = doComplexSolid(objectX[id], objectY[id], objData3[id], objectX[id] + objXL[1], objectY[id] + objYL[1], objectX[i], objectY[i], objectX[i] + objXL[current], objectY[i] + objYL[current]);

				if (rObj.y != objectY[id])
					objData2[id] = 0; // ak spadol na zem, zastavit jeho rychlost

				objectX[id] = rObj.x;
				objectY[id] = rObj.y;
			}

		}
		i++;
	}

	rObj.x = objectX[id] - camx;
	rObj.y = objectY[id] - camy;

	if (rObj.x > -300 && rObj.x < 640) {
		if (animFrame[id] > 8)
			animFrame[id] = 1;

		if (animFrame[id] == 5 && pos.x + 46 < objectX[id] + 50)
			rObj.x -= (53-46);

		if (pos.x + 46 < objectX[id] + 50)
			SDL_BlitSurface(ironStick, NULL, myscreen, &rObj); // draw object
		else
			SDL_BlitSurface(ironStick2, NULL, myscreen, &rObj);

		if (animFrame[id] > 1)
			animFrame[id]++;
		if (animFrame[id] > 8)
			animFrame[id] = 1;
	}

	if (animFrame[id] < 0 || animFrame[id] > 8 || !animFrame[id])
		animFrame[id] = 1;
	if (objectT[id] == 101)
		objData1[id] = 10;

	objData3[id] = objectX[id];
}