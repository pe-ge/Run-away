#include "libraries.h"
#include "dec.h"
#include "functions.h"
#include "processObjects.h"
#include "CMenu.h"
#include "init.h"
#include "collision.h"

void mainCycle(CompleteMenu &m) {
	
	menuMode = true;
	Mix_PlayChannel(0, overworld, -1);
	Mix_Pause(0);
	while (true) {
		
		if (menuMode){
			SDL_FillRect(myscreen, NULL, SDL_MapRGB(myscreen->format, 0, 255, 128));
			if (!processEventsInMenu(m))
				return;

			m.draw(myscreen);
		} else {
			if (!isPlayingMusic && musicStatus) {
				Mix_Resume(0);
				isPlayingMusic = true;
			}

			if (((ticks - start_ticks) < (1000 / FPS)))
				ticks = SDL_GetTicks();
			else {
				ESt = SDL_GetTicks();
				ticks = SDL_GetTicks();
				start_ticks = SDL_GetTicks();

				ManageCamera();
				ManagePlayer();
				ManageCheats();
				DrawDecor();
				if(!ManageMisc()) {
					cerr << "ManageMisc() failed!" << endl;
					return;
				}
				ManageGameOver();
				ManagePowerBar();
			}			
		}
		SDL_Flip(myscreen);
	}
}

int SDL_main(int argc, char *argv[]) {
	if (!initGame()) {
		getchar();
		return -1;
	}

	CompleteMenu m;
	createMenu(m);
	
	mainCycle(m);

	quitGame();

	return 0;
}

bool objectsProgram() {
	int id = 0; //iterator vo while cykle
	while (id < numberOfObjects) {
		id++;

		rObj.x = objectX[id] - camx;
		rObj.y = objectY[id] - camy;

		//ak je daleko od hraca - nespracuvat
		if (!(rObj.x > -1000 && rObj.x < 1000))
			continue;
		
		if (objectY[id] > 500)
			objectY[id] = 500;

		if (objectT[id] == 1) {
			processStick(id);
		}

		if (objectT[id] == 2) {
			processGunstick(id);
		}

		if (objectT[id] == 5) {
			processTrampoline(id);
		}

		if (objectT[id] == 6) {
			processBallOnGround(id);
		}

		if (objectT[id] == 7) {
			processStandardPlatform(id);
		}

		if (objectT[id] == 8) {
			processWall(id);
		}

		if (objectT[id] == 9) {
			processFlippingPlatform(id);
		}

		if (objectT[id] == 10) {
			processLean(id);
		}

		if (objectT[id] == 11) {
			processOffScrolling(id);
		}

		if (objectT[id] == 12) {
			processOnScrolling(id);
		}
		
		if (objectT[id] == 14) {
			processLowerSpike(id);
		}

		if (objectT[id] == 15) {
			processUpperSpike(id);
		}

		if (objectT[id] == 18) {
			processMovingPlatform(id);
		}

		if (objectT[id] == 20) {
			processAnimation(id);
		}

		if (objectT[id] == 101) {
			processDyingStick(id);
		}

		if (objectT[id] == 106) {
			processPickedBall(id);
		}

		if (objectT[id] == 109) {
			processFlippingPlatform2(id);
		}

		if (objectT[id] == 156) {
			processThrownBall(id);
		}

		if (objectT[id] == 209) {
			processFlippingPlatform3(id);
		}
	}

	//ak sme dosli na koniec mapy
	if (currentLevel == firstLevel && pos.x > firstEndX) {
		currentLevel = secondLevel;
		restartCurrentLevel();
	} else if (currentLevel == secondLevel && pos.x > secondEndX) {
		currentLevel = thirdLevel;
		restartCurrentLevel();
	}
	
	return true;
}

void ManageCamera() {
	if (CameraMode == CM_SEEK_DOOR) {

		int previousCamX = camx;
		if (toDoorX - 320 > camx)
			camx += SEEKER_CAM_SPD;

		if (toDoorX - 320 < camx)
			camx -= SEEKER_CAM_SPD;

		if (camx < 0)
			camx = 0;

		if (camx == previousCamX) {
			pos.x = toDoorX;
			pos.y = toDoorY;

			normalPlayerControls = true;
			CameraMode = CM_FOLLOW_PLAYER;

		} else
			PlayerAlpha = 0;
	}

	if (CameraMode == CM_FOLLOW_PLAYER) {
		if (pos.y - camy < VERTICAL_SCROLL_TOP) {
			int dy = VERTICAL_SCROLL_TOP - (pos.y - camy);

			if (verticalScrolling)
				camy -= dy;
		}

		if (pos.y - camy > VERTICAL_SCROLL_BOTTOM) {
			int dy = pos.y - camy - VERTICAL_SCROLL_BOTTOM;

			Camera_SpeedDown++;

			if (Camera_SpeedDown > VERTICAL_SCROLL_MAXSPEED_DOWN)
				Camera_SpeedDown = VERTICAL_SCROLL_MAXSPEED_DOWN;

			if (Camera_SpeedDown < 0)
				Camera_SpeedDown = 0;

			if (verticalScrolling)
				camy += dy;
		} else
			Camera_SpeedDown = 0;

		if (camy > 0)
			camy = 0;
	}

	if (!verticalScrolling) {
		if (camy < 0)
			camy += 8;

		if (camy > 0)
			camy = 0;
	}

	if (camx == 0) {
		plane1cx = 0;
		plane2cx = 0;
		plane3cx = scroller3;
	}

	scroller3 += AUTOSCROLL_3;
	if (scroller3 > 639)
		scroller3 = 0;
}

void CheapTrace(string msg, int fps) {
	string caption = msg;

	if (fps != 0)
		caption += toString(fps);

	SDL_WM_SetCaption(caption.c_str(), NULL);
}

void killEnemy(int id) {
    objData1[id] = 10;
    objectT[id] = 101;
}

//hodi hraca na danu poziciu
void WarpPlayerTo(int location_x, int location_y) {
	PlayerAlpha = 255;
	toDoorX = location_x;
	toDoorY = location_y - 5;

	pos.x = location_x;
	pos.y = location_y;
}

int giveJumpPulse(int power) {
	if (!jumping && !fallRecorded) {
		if (fallSpeed == 0) {
			jumping = true;
			jpf = power;
			return true;
		} else
			return false;
	} else
		return false;
}

void forceJumpPulse(int power) {
	jumping = true;
	jpf = power;
}

void DrawDecor() {

	//scrollovanie kamery
	// sirka playera = 46
	int ocamx = camx;

	if (CameraMode != CM_STATIC && camx < 0)
		camx = 0;

	if (CameraMode == CM_FOLLOW_PLAYER) {
		if (pos.x - camx + 23 > RightScrollPoint)
			camx += pos.x - camx - RightScrollPoint + 23;

		int dx = pos.x - camx;

		// to the left (middle)
		if (dx < LeftScrollPoint)
			camx -= LeftScrollPoint - dx;

		if (camx < 0)
			camx = 0;

		if (pos.x < 0)
			pos.x = 0;
	}

	if (AlphaSpinMode == 1)
		AlphaSpin -= 25;
	else
		AlphaSpin += 25;

	if (AlphaSpin > 256) {
		AlphaSpin = 255;
		AlphaSpinMode = 1;
	}

	if (AlphaSpin < 0) {
		AlphaSpin = 0;
		AlphaSpinMode = 2;
	}

	SDL_Rect fullScreen;
	fullScreen.x = 0;
	fullScreen.y = 0;
	fullScreen.w = 640;
	fullScreen.h = 480;// - 179 - camy + 10;

	Uint32 whiteColor = SDL_MapRGB(myscreen->format, 255, 255, 255);
	SDL_FillRect(myscreen, &fullScreen, whiteColor);

	// MULTI-PLANE BACKGROUND

	// LAYER 3---------------------

	plane3cx += AUTOSCROLL_3;
	int next = -plane3cx;

	while (next < 640) {
		plane3.x = next;
		plane3.y = -camy;

		SDL_BlitSurface(sPlane3, NULL, myscreen, &plane3);
		next += sPlane3->w;
	}

	if (plane3cx > sPlane3->w)
		plane3cx = 0;

	if (plane3cx < 0)
		plane3cx = sPlane3->w;

	plane3.x = -plane3cx;

	// LAYER 2---------------------
	next = -plane2cx;

	while (next < 640) {
		plane2.x = next;
		plane2.y = -camy + 480 - 377;

		SDL_BlitSurface(sPlane2, NULL, myscreen, &plane2);
		next += sPlane2->w;
	}

	if (plane2cx > sPlane2->w)
		plane2cx = 0;

	if (plane2cx < 0)
		plane2cx = sPlane3->w;

	plane2.x = -plane2cx;

	fullScreen.x = 0;
	fullScreen.y = -camy + 480 - 377 + sPlane2->h;
	fullScreen.w = 640;
	fullScreen.h = 30;

	SDL_FillRect(myscreen, &fullScreen, whiteColor);

	// LAYER 1---------------------
	next = -plane1cx;

	while (next < 640) {
		plane1.x = next;
		plane1.y = -camy + 480 - 239;
		SDL_BlitSurface(sPlane1, NULL, myscreen, &plane1);
		next += sPlane1->w;
	}

	if (plane1cx > sPlane1->w)
		plane1cx = 0;

	if (plane1cx < 0)
		plane1cx = sPlane3->w;

	plane1.x = -plane1cx;

	fullScreen.x = 0;
	fullScreen.y = 480 - 239 + sPlane1->h - camy;
	fullScreen.w = 640;
	fullScreen.h = 239 - sPlane1->h;

	SDL_FillRect(myscreen, &fullScreen, whiteColor);


	if (-plane1cx + sPlane1->w < 640) {
		plane1.x = -plane1cx + sPlane1->w;
		plane1.x = -plane1cx;
	}

	// PLANE 1 -------------------

	int dx = 0;

	if (ocamx > camx) {
		// left
		dx = ocamx - camx;
		dx /= PLANE1_FACTOR;
		dx *= -1;
	} else {
		// right
		dx = camx - ocamx;
		dx /= PLANE1_FACTOR;
	}

	plane1cx += dx;
	plane1.x = -plane1cx;
	plane1.y = 0;

	// PLANE 2 -------------------

	dx = 0;

	if (ocamx > camx) { //nalavo
		dx = ocamx - camx;
		dx /= PLANE2_FACTOR;
		dx *= -1;
	} else { //napravo
		dx = camx - ocamx;
		dx /= PLANE2_FACTOR;
	}

	plane2cx += dx;
	plane2.x = -plane2cx;
	plane2.y = 0;

	// PLANE 3 -------------------

	dx = 0;

	if (ocamx > camx) { //nalavo
		dx = ocamx - camx;
		dx /=PLANE3_FACTOR;
		dx *= -1;
	} else { //napravo
		dx = camx - ocamx;
		dx /= PLANE3_FACTOR;
	}

	plane3cx += dx;
	plane3.x = -plane3cx;
	plane3.y = 0;
}

bool restartCurrentLevel() {
	plane1cx = 0;
	camx = 0;
	plane2cx = 0;
	plane3cx = 0;

	plane1.x = 0;
	plane2.x = 0;
	plane3.x = 0;

	pos.x = 0;
	pos.y = 0;

	pwr = 50;

	CameraMode = CM_FOLLOW_PLAYER;
	verticalScrolling = true;

	numberOfObjects = LoadMap(currentLevel);
	if (numberOfObjects == 0)
		return false;
	else
		return true;
}

void ManagePlayer() {
	SDL_PumpEvents();
	keystate = SDL_GetKeyState(NULL);

	if (gameOverJumped || normalPlayerControls == false) {
		keystate[SDLK_LEFT] = 0;
		keystate[SDLK_RIGHT] = 0;
		keystate[SDLK_UP] = 0;
		keystate[SDLK_DOWN] = 0;
		keystate[SDLK_ESCAPE] = 0;
	}

	if (keystate[SDLK_ESCAPE]) {
		Mix_Pause(0);
		menuMode = true;
		isPlayingMusic = false;
		return;
	}

	if (keystate[SDLK_LEFT]) {
		isFacingRight = false;
		if (runLeftSpeed < DEF_LFORCE)
			runLeftSpeed += 2;
	}

	if (keystate[SDLK_RIGHT]) {
		isFacingRight = true;
		if (runRightSpeed < DEF_LFORCE)
			runRightSpeed += 2;
	}

	pos.x += runRightSpeed;
	pos.x -= runLeftSpeed;

	int slideDelay = 80;
	int slideTime = 5;
	static int TICK_X47 = 0;

	if (bar <= slideDelay)
		TICK_X47 = slideTime;

	if (TICK_X47 == slideTime) {
		TICK_X47 = 0;
		if (runRightSpeed > 0)
			runRightSpeed--;
		if (runLeftSpeed > 0)
			runLeftSpeed--;
	} else
		TICK_X47++;

	if (jumping)
		if (!keystate[SDLK_UP])
			if (jpf < JPOWER * 0.9)
				jpf--;

	if (keystate[SDLK_LEFT]) {
		running = true;
		direction = LEFT;
		currentRunFrame = (currentRunFrame + 1) % 12;
	} else if (keystate[SDLK_RIGHT]) {
		running = true;
		direction = RIGHT;
		currentRunFrame = (currentRunFrame + 1) % 12;
	} else {
		running = false;
	}

	if (keystate[SDLK_UP]) {
		if (!jumping && !fallRecorded)
			if (!upWasDown)
				if (fallSpeed == 0) {
					if (effectsStatus)
						Mix_PlayChannel(1, jump, 0);
					upWasDown = true;
					jumping = true;
					jpf = JPOWER;
					TimeVelocityGained = 0;
				}
	} else
		upWasDown = false;

	if (fallSpeed > 2)
		TimeVelocityGained = 0;

	if (runLeftSpeed > DEF_LFORCE - 2 || runRightSpeed > DEF_LFORCE - 2) {
		TimeVelocityGained++;
		JPOWER = DEF_JPOWER;
		if (TimeVelocityGained > SUPER_SPEED_DELAY_F_SEC * FPS)
			JPOWER = DEF_JPOWER + 5;
	} else {
		JPOWER = DEF_JPOWER;
		TimeVelocityGained = 0;
	}

	if (jumping)
		if (jpf > 0) {
			pos.y -= jpf;
			jpf--;
		} else
			jumping = false;

	fallRecorded = 0;

	if (!jumping && CameraMode != CM_SEEK_DOOR) {
		pos.y += fallSpeed;
		if (fallSpeed < JPOWER + 1)
			fallSpeed++;
	} else
		fallSpeed = 0;
}

void ManageCheats() {
	if (!DEVELOPER) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN:
					if(cheat_str[cheat_pos] == event.key.keysym.sym) {
						if(++cheat_pos == strlen(cheat_str)) {
							cout << "CHEAT ACTIVATED" << endl;
							DEVELOPER = true;
						}
					} else
						cheat_pos = 0;
				break;
			}
		}
		return;
	}

	//zmena zivota
	if (keystate[SDLK_1])
		pwr--;
	if (keystate[SDLK_2])
		pwr++;

	//skok
	if (keystate[SDLK_j])
		forceJumpPulse(20);

	//reset objektov
	if (keystate[SDLK_o]) {
		numberOfObjects = LoadMap(currentLevel);
		if(!numberOfObjects)
			return;
	}

	//rychly pohyb dopredu
	if (keystate[SDLK_y]) {
		pos.y = -playerIdleLeft->h - 10;
		camy = 0;
		pos.x +=100;
	}

	//rychly pohyb dozadu
	if (keystate[SDLK_t]) {
		pos.y = -playerIdleLeft->h - 10;
		camy = 0;
		pos.x -= 100;
	}

	//ulozenie hracovej pozicie
	if (keystate[SDLK_f]) {
		saveX = pos.x;
		saveY = pos.y;
	}

	//spawnutie hraca na ulozenu poziciu
	if (keystate[SDLK_g])
		WarpPlayerTo(saveX,saveY);

	//dalsie kolo
	if (keystate[SDLK_a]) {
		if (!aWasDown) {
			aWasDown = true;
			
			if (currentLevel == firstLevel)
				currentLevel = secondLevel;
			else if (currentLevel == secondLevel)
				currentLevel = thirdLevel;
			restartCurrentLevel();
		}
	} else
		aWasDown = false;

	//restart hry
	if ((keystate[SDLK_r])) {
		currentLevel = firstLevel;
		restartCurrentLevel();
	}

	//screenshot
	if (keystate[SDLK_s]) {	
		if (sWasDown == false) {
			sWasDown = true;
			SDL_SaveBMP(myscreen, "screen.bmp");
		}
	} else
		sWasDown = false;
}

bool ManageMisc() {
	//ked som skapal
	if (pwr < 1 && pos.y > 300 || pos.y > 480 && pwr > 1) {
		return restartCurrentLevel();
	}

	if(!objectsProgram()) {
		cerr << "numberOfObjectsProgram() failed! " << endl;
		return false;
	}

	if (fallSpeed > 1)
		fall_timer++;
	else
		fall_timer = 1;
	
	SDL_Rect dpos;
	dpos.x = pos.x - camx;
	dpos.y = pos.y - camy;
	//kreslenie playera pri skakani/padani
	if (jumping)
		if (gameOverJumped)
			if (direction == RIGHT)
				SDL_BlitSurface(fallRight, NULL, myscreen, &dpos); //ked skapem -> sprite vyskocenia
			else
				SDL_BlitSurface(fallLeft, NULL, myscreen, &dpos);
		else
			if (direction == RIGHT)
				SDL_BlitSurface(jumpRight, NULL, myscreen, &dpos);
			else
				SDL_BlitSurface(jumpLeft, NULL, myscreen, &dpos);
	else if (fallSpeed > 1)
		if (direction == RIGHT)
				SDL_BlitSurface(fallRight, NULL, myscreen, &dpos);
			else
				SDL_BlitSurface(fallLeft, NULL, myscreen, &dpos);
	else {
		if (direction == RIGHT)
			if (running)
				SDL_BlitSurface(playerRunRight[currentRunFrame], NULL, myscreen, &dpos);
			else
				SDL_BlitSurface(playerIdleRight, NULL, myscreen, &dpos); //panacik v klude na zemi otoceny vpravo
		else
			if (running)
				SDL_BlitSurface(playerRunLeft[currentRunFrame], NULL, myscreen, &dpos);
			else
				SDL_BlitSurface(playerIdleLeft, NULL, myscreen, &dpos); //panacik v klude na zemi otoceny vlavo
	}

	ESt2 = SDL_GetTicks();
	int execSpeed = ESt2 - ESt;

	if (keystate[SDLK_F1])
		CheapTrace("Run away - FPS :", 1000 / execSpeed);

	if (keystate[SDLK_F2]) {
		CheapTrace("Run away", 0);
	}

	return true;
}

void ManagePowerBar() {
	int x1 = 9;
	int x2 = 252;
	int sizeOfBar = x2 - x1;

	int y2 = 47+2;

	int smx = x1;
	int smy = 39;

	while (smy < y2) {
		smx = x1;
		while (smx < x2){
			Uint32 PixelColor = SDL_MapRGB(myscreen->format, 0, 0, 0);
			putPixel(bar2, smx, smy, PixelColor);
			smx++;
		}
		smy++;
	}

	if (TimeVelocityGained > BAR2_GFX_LIMIT && !jumping && fallSpeed < 2) {
		if (TimeVelocityGained > bar)
			bar++;
	} else if (bar > 0)
		bar -= 2;	

	int length = (int)(bar / (SUPER_SPEED_DELAY_F_SEC * FPS));

	if (length > 1)
		length = 1;

	if (TimeVelocityGained < SUPER_SPEED_DELAY_F_SEC * FPS)
		length = 0;
	else
		length = 1;

	length *= sizeOfBar;

	if (length < 0)
		length = 0;

	x2 = 9 + length;

	y2 = 47 + 2;

	smx = x1;
	smy = 39;

	while (smy < y2) {
		smx = x1;
		while (smx < x2) {
			Uint32 PixelColor = SDL_MapRGB(bar2->format, 255, 0, 0);
			putPixel(bar2, smx, smy, PixelColor);
			PixelColor = SDL_MapRGB(myscreen->format, 0, 0, 0);
			putPixel(bar2, smx, 39, PixelColor);
			putPixel(bar2, smx, y2, PixelColor);

			smx++;
		}

		Uint32 PixelColor = SDL_MapRGB(myscreen->format, 0, 0, 0);
		putPixel(bar2, x1, smy, PixelColor);
		putPixel(bar2, x2, smy, PixelColor);

		smy++;
	}

	SDL_SetAlpha(bar2, SDL_SRCALPHA, AlphaSpin);

	if (TimeVelocityGained > SUPER_SPEED_DELAY_F_SEC * FPS) {
		SDL_Rect pointZero;
		pointZero.x = 0;
		pointZero.y = 0;
		SDL_BlitSurface(bar2, NULL, myscreen, &pointZero);
	}

	if (pwr < 20) {
		ALPHA_CYCLE = ALPHA_CYCLE + 0.1;
		if (ALPHA_CYCLE > 1 && sin(ALPHA_CYCLE) < 0.1)
			ALPHA_CYCLE = 0;

	} else
		ALPHA_CYCLE = 1.5;

	if (pwr > 50)
		pwr = 50;

	if (pwr < 0)
		pwr = 0;

	int c2 = pwr * barlen / 50;
	int pwrb = pwr * 5;

	if (c2 > barlen)
		c2 = barlen;
	if (c2 < 1)
		c2 = 1;

	SDL_Rect fullScreen;
	fullScreen.x = 10;
	fullScreen.y = 16;
	fullScreen.w = c2 - 1;
	fullScreen.h = barleny + 1;

	Uint32 PixelColor = SDL_MapRGB(pbar->format, 250 - pwrb, pwrb, 0);
	SDL_FillRect(pbar, &fullScreen, PixelColor);

	fullScreen.x = 10 + c2;
	fullScreen.y = 16;
	fullScreen.w = barlen - c2 - 1;
	fullScreen.h = barleny + 1;

	Uint32 whiteColor = SDL_MapRGB(pbar->format, 255, 255, 255);
	SDL_FillRect(pbar, &fullScreen, whiteColor);

	if (pwr < 27)
		SDL_SetAlpha(pbar, SDL_SRCALPHA, AlphaSpin);
	else
		SDL_SetAlpha(pbar, SDL_SRCALPHA, 255);

	SDL_Rect pointZero;
	pointZero.x = 0;
	pointZero.y = 0;
	SDL_BlitSurface(pbar, NULL, myscreen, &pointZero);
}

void ManageGameOver() {
	if (pwr < 1) {
		if (gameOverJumped == 0) {
			CollisionEngine_On = false; //vypneme kolizie pri padani, aby sme sa mohli prepadnut zemou
			forceJumpPulse(25);
			gameOverJumped = 1;
		}
	} else
		gameOverJumped = 0;

	if (gameOverJumped){
		if (!jumping) {
			gameOverJumped = 2;
			CameraMode = CM_STATIC;
			CollisionEngine_On = false;
		}
	}
}