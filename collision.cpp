#include "collision.h"

//zisti kolizie s ocelovymi platformami - ci sa moze na ne postavit
void programSolid(int objx1, int objy1, int objx2, int objy2) {
	int prevPos;

	int epx = pos.x + playerIdleLeft->w;
	int epy = pos.y + playerIdleLeft->h;

	//ak zboku narazim na platforum
	if (epx > objx1 && pos.x < objx2)
		if (epy - 3 > objy1 && pos.y + 3 < objy2) {

			if (isFacingRight)
				prevPos = LEFT;
			else
				prevPos = RIGHT;

			if (prevPos == LEFT) {
				if (pos.x < objx1 - 23) {
					int dx = epx - objx1;
					pos.x -= dx;
				}
			} else if (epx > objx2 + 23) {
				int dx = objx2 - pos.x;
				pos.x += dx;
			}
		}
	
	//ak zdola narazim na platformu
	if (jumping)
		if (epx - 23 > objx1 && pos.x + 23 < objx2)
			if (epy > objy2 && pos.y < objy2) {
				jpf = 0;
				jumping = false;
				pos.y = objy2;
			}

	//ak dopadnem na platformu
	if (epy > objy1)
		if (epy < objy2)
			if (pos.y < objy1)
				if (epx > objx1)
					if (pos.x < objx2)
						if (!jumping)
							if (fallSpeed > 0) {
								//if (ljy + playerIdleLeft->h < objy1) {
									int dy = epy - objy1;
									pos.y -= dy;
									fallRecorded = 0;
									fallSpeed = 0;
								}
}

SDL_Rect doComplexSolid(int subjx, int subjy, int subj_lx, int subjex, int subjey, int objectx, int objecty, int objectex, int objectey) {
	SDL_Rect subject;
	SDL_Rect subjecte;
	SDL_Rect object;
	SDL_Rect objecte;
	int prevPos;
	int halfSubj;

	halfSubj = subjex - subjx;
	halfSubj = halfSubj / 2;

	object.x = objectx;
	object.y = objecty;

	objecte.x = objectex;
	objecte.y = objectey;

	subject.x = subjx;
	subject.y = subjy;

	subjecte.x = subjex;
	subjecte.y = subjey;

	if (subjecte.x > object.x && subject.x < objecte.x)
		if (subjecte.y - 3 > object.y && subject.y + 3 < objecte.y) {
			//zisti, ci player bol nalavo alebo napravo pred koliziou
			if (subj_lx > subject.x)
				prevPos = RIGHT;
			else
				prevPos = LEFT;

			if (prevPos == LEFT) {

				if (subject.x < object.x - halfSubj)
					subject.x = subject.x - (subjecte.x - object.x);
			} else 
				if (subjecte.x > objecte.x + 23)
					subject.x = subject.x + (objecte.x - subject.x);
		}

	if (subjecte.x - halfSubj > object.x && subject.x + halfSubj < objecte.x)
		if (subjecte.y > objecte.y && subject.y < objecte.y)
			subject.y = objecte.y;

	if (subj_lx + playerIdleLeft->w > object.x && subj_lx < objecte.x)
		if (subjecte.y > object.y)
			if (subjecte.y < objecte.y)
				if (subject.y < object.y)
					if (subjecte.x > object.x)
						if (subject.x < objecte.x) {
							subject.y -= subjecte.y - object.y;
						}
	return subject;
}

int doComplexStickmanPlatform (int obj1x, int obj1y, int obj_ljy, int obj1ex, int obj1ey, int obj2x, int obj2y, int obj2ex, int obj2ey) {
	if (CollisionEngine_On)
		if (obj1ey > obj2y)
			if (obj1ey < obj2ey)
				if (obj1y < obj2y)
					if (obj1ex > obj2x)
						if (obj1x < obj2ex)
							if (obj_ljy < obj2y) {
								int complexPlatformTest = obj1ey - obj2y;
								return obj1y - complexPlatformTest;
							}
	return obj1y;
}

//kolizie protivnikov s prostredim
int doComplexPlatform(int obj1x, int obj1y, int obj1ex, int obj1ey, int obj2x, int obj2y, int obj2ex, int obj2ey) {
	if (obj1ey > obj2y)
		if (obj1ey < obj2ey)
			if (obj1y < obj2y)
				if (obj1ex > obj2x)
					if (obj1x < obj2ex) {
						int complexPlatformTest = obj1ey - obj2y;
						return obj1y - complexPlatformTest;
					}
	return obj1y;
}

//kolizie pri stupajucej ceste
void programLean(int objectx, int objecty, int objectex, int objectey) {
	int epx = pos.x + playerIdleLeft->w;
	int epy = pos.y + playerIdleLeft->h;
	if (CollisionEngine_On) {
		if (flipDown > 0)
			flipDown--;

		if (pos.y + playerIdleLeft->h > flipEnd)
			flipDown = 0;

		if (flipDown == 0) {
			if (epy > objecty - 5 && epy < objecty + 20)
				if (epx > objectx)
					if (pos.x < objectex)
						if (!jumping)
							if (fallSpeed > 0) {
								if (keystate[SDLK_DOWN]) {
									if (!doWasDown && flipDown == 0) {
										doWasDown = 1;
										flipDown = 10000;
										flipEnd = objectey;
									}
								} else
									doWasDown = 0;

								int dy = epy - objecty;
								pos.y -= dy;
								fallRecorded = 0;
								fallSpeed = 0;
							}
		}
	}
}

//spravanie na zemi a trampoline
//parametre = velkost objektu
bool programPlatform(int objx1, int objy1, int objx2, int objy2) {
	int epx = pos.x + playerIdleLeft->w;
	int epy = pos.y + playerIdleLeft->h;
	if (CollisionEngine_On) {
		if (flipDown > 0)
			flipDown--;

		if (epy > flipEnd)
			flipDown = 0;

		if (flipDown == 0) {

			if (epy > objy1)
				if (epy < objy2)
					if (pos.y < objy1)
						if (epx > objx1)
							if (pos.x < objx2)
								if (!jumping)
									if (fallSpeed > 0) {
											if (keystate[SDLK_DOWN]) {
												if (!doWasDown && flipDown == 0) {
													doWasDown = 1;
													flipDown = 10000;
													flipEnd = objy2;

													if (callMovingPtf)
														flipEnd = vertMoveEnd + DEF_JPOWER;

													callMovingPtf = false;
												}
											} else
												doWasDown = 0;

											int dy = epy - objy1;
											pos.y -= dy;
											fallRecorded = 0;
											fallSpeed = 0;

											callMovingPtf = false;
											return true;
										}
			callMovingPtf = false;
			return false;
		}
	}
	callMovingPtf = false;
	return false;
}