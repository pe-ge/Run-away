#include "processObjects.h"

void processGunstick(int id){
	//ak dopadneme nepriatelovi na hlavu
	if (programPlatform(objectX[id], objectY[id], objectX[id] + 75, objectY[id] + 21)) {
		objectT[id] = 101; //nastavit jeho smrt
		giveJumpPulse(DEF_JPOWER + 1); //vyskocit
		if (effectsStatus)
			Mix_PlayChannel(1, jump, 0);
	}

	if (objectX[id] - camx > 0 && objectX[id] - camx < 640) { //je viditelny?
		if (pos.x + 50 + 150 < objectX[id])
			objectX[id] -= 5;

		if (pos.x - 150 > objectX[id] + objXL[2])
			objectX[id] += 5;

		float bullet_delay = FPS / BPS;

		if (pos.y > objectY[id] - 300 && pos.y < objectY[id] + objYL[2] + 300) {
			// strelba
			
			objData4[id]++;
			if (bullets[id] < MAX_ATTACKS && objData4[id] > bullet_delay) {
				objData4[id] = 0;
				
				if (bullets[id] < 0)
					bullets[id] = 0;

				bool bulletFound = false;
				int lastBullet = 0;
				int i = 1;
				while (i < MAX_ATTACKS + 1) {
					int bulletId = id * MAX_ATTACKS;
					bulletId += i;
					if (bulletsO[bulletId] == false) {
						bulletFound = true;
						lastBullet = i;
					}
					i++;
					if (effectsStatus)
						Mix_PlayChannel(2, fire, 0);
				}

				if (bulletFound) {
					int bulletId = id * MAX_ATTACKS;
					bulletId += lastBullet;

					if (pos.x < objectX[id])
						bulletsX[bulletId] = objectX[id];
					else
						bulletsX[bulletId] = objectX[id] + objXL[objectT[id]];

					bulletsY[bulletId] = objectY[id] + 12;

					if (pos.x < objectX[id])
						bulletsXD[bulletId] = -BULLET_PPF;
					else
						bulletsXD[bulletId] = BULLET_PPF;

					bulletsO[bulletId] = true;
					bullets[id]++; //vystrelil
					bulletLife[bulletId] = 100;
				} 
			}
		}
	}

	//naboje
	if (bullets[id] > 0) {
		int i = 1;
		while (i < MAX_ATTACKS + 1) {
			int index = id * MAX_ATTACKS;
			index += i;

			if (bulletsO[index] == true) {
				bulletsX[index] += bulletsXD[index];
				bulletLife[index]--;

				if (bulletsX[index] < 0 || bulletLife[index] < 0 || bulletsX[index] - camx > 640 || bulletsX[index] - camx < 0) {
					//znic naboj
					bulletsO[index] = false;
					bullets[id]--;
				}

				//ak ma trafi gulka
				if (collides(bulletsX[index], bulletsY[index], bullet->w, bullet->h, pos.x, pos.y, playerIdleLeft->w, playerIdleLeft->h)) {
					bulletsO[index] = false;
					bullets[id]--;
					pwr -= BULLET_HP_DRAIN;
				}

				int i = 1;
				while (i < numberOfObjects) {
					if (i != id) {
						int typeObj = objectT[i];

						//ak trafi gulka nejaky objekt -> zrusime ju
						if (collides(bulletsX[index], bulletsY[index], bullet->w, bullet->h, objectX[i], objectY[i], objXL[typeObj], objYL[typeObj])) {
							bulletsO[index] = false;
							bullets[id]--;
						}
					}
					i++;
				}

				myObjectPos.x = bulletsX[index] - camx;
				myObjectPos.y = bulletsY[index] - camy;

				SDL_BlitSurface(bullet, NULL, myscreen, &myObjectPos);
			}
			i++;
		}
	}

	objectY[id] += objData2[id];
	if (objData2[id] < 10)
		objData2[id]++;
	
	int i = 1;
	while (i < numberOfObjects) {
		if (i != id) {
			int current = objectT[i];
			if (typeDef[current] == TYPE_PLATFORM) {
				int tmp = doComplexPlatform(objectX[id], objectY[id], objectX[id] + objXL[2], objectY[id] + objYL[2], objectX[i], objectY[i], objectX[i] + objXL[current], objectY[i] + objYL[current]);
				if (tmp != objectY[id])
					objData2[id] = 0;
				objectY[id] = tmp;
			}

			//kolizia nepriatelov
			if (typeDef[current] == TYPE_SOLID || typeDef[current] == TYPE_ENEMY) {
				current = objectT[i];
				rObj = doComplexSolid(objectX[id], objectY[id], objData3[id], objectX[id] + objXL[2], objectY[id] + objYL[2], objectX[i], objectY[i], objectX[i] + objXL[current], objectY[i] + objYL[current]);
				if (rObj.y != objectY[id])
					objData2[id] = 0;
				objectX[id] = rObj.x;
				objectY[id] = rObj.y;
			}
		}
		i++;
	}

	rObj.x = objectX[id] - camx;
	rObj.y = objectY[id] - camy;

	if (rObj.x > -300 && rObj.x < 640)
		if (objectX[id] < pos.x)
			SDL_BlitSurface(gunStick2, NULL, myscreen, &rObj); 
		else
			SDL_BlitSurface(gunStick, NULL, myscreen, &rObj);
	if (objectT[id] == 101)
		objData1[id] = 10;

	// posledne x
	objData3[id] = objectX[id];
}