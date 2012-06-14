#include "init.h"

bool initGame() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cerr << "SDL_Init failed!" << endl;
		return false;
	}

	if(TTF_Init() == -1){
		cerr << "TTF_Init failed " << SDL_GetError() << endl;
		getchar();
		return false;
	}

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		cerr << "Mix_OpenAudio failed " << SDL_GetError() << endl;
		return false;
	}

	if (!loadSounds()) {
		cerr << "loadSounds() failed !" << endl;
		return false;
	}

	if (!InitVideoStuff()) {
		cerr << "InitVideoStuff failed!" << endl;
		return false;
	}
	if (!SetUpEngine()) {
		cerr << "SetUpEngine() failed!" << endl;
		return false;
	}
	if (!getImagesLength()) {
		cerr << "getImagesLength() failed !" << endl;
		return false;
	}
	//startup
	if (!loadObjectData("Data/Engine/objects.obj")) {
		cerr << "loadObjectData(\"Data/Engine/objects.obj\") failed !" << endl;
		return false;
	} 

	if(!SetUpGame()) {
		cerr << "SetUpGame() failed!" << endl;
		return false;
	}

	if(!ConfigGame()) {
		cerr << "ConfigGame() failed!" << endl;
		return false;
	}

	return true;
}

bool loadSounds() {
	if((overworld = Mix_LoadWAV("Data/Music/overworld.wav")) == NULL ||
	   (punch = Mix_LoadWAV("Data/Music/punch.wav")) == NULL ||
	   (jump = Mix_LoadWAV("Data/Music/jump.wav")) == NULL ||
	   (fire = Mix_LoadWAV("Data/Music/fire.wav")) == NULL)
		return false;

	Mix_AllocateChannels(12);

	return true;
}

void createMenu(CompleteMenu &m){
	MenuFont3 = TTF_OpenFont("Data/fonts/saunder.ttf", 50);
	MenuFont4 = TTF_OpenFont("Data/fonts/saunder.ttf", 40);

	if (!MenuFont3 || !MenuFont4){
		cerr << "Loading fonts failed: " << SDL_GetError() << endl;
		return;
	}

	SDL_Surface *bgMenu = NULL;
	PageInMenu MainPage(*bgMenu, *MenuFont3);
	MainPage.addLine("Start game");
	MainPage.addLine("Options");
	MainPage.addLine("About");
	MainPage.addLine("Exit");

	PageInMenu Options(*bgMenu, *MenuFont3);
	Options.addLine("Graphics");
	Options.addLine("Sound");
	Options.addLine("Keyboard");
	Options.addLine(" ");
	Options.addLine("Return");

	PageInMenu About(*bgMenu, *MenuFont3);
	About.addLine("Run away version 0.9b");
	About.addLine("Created by Peter Gergel");
	About.addLine(" ");
	About.addLine("Controls");
	About.addLine("Movement - arrows");
	About.addLine("Return to menu - Esc");

	PageInMenu Graphics(*bgMenu, *MenuFont3);
	Graphics.addLine("Resolution");
	Graphics.addLine("Color Depth");
	if (SDL_FULLSCREEN & SDL_DOUBLEBUF){
		Graphics.addLine("Fullscreen - Yes");
	} else{
		Graphics.addLine("Fullscreen - No");
	}
	Graphics.addLine(" ");
	Graphics.addLine("Return");

	PageInMenu Resolution(*bgMenu, *MenuFont4);

	SDL_Rect** modes = SDL_ListModes(NULL, SDL_DOUBLEBUF|SDL_FULLSCREEN);
	for (int i = 0; modes[i]; i++){
		stringstream st; 
		st << modes[i]->w << "x" << modes[i]->h;
		Resolution.addLine(st.str());
	}

	Resolution.addLine(" ");
	Resolution.addLine("Return");

	PageInMenu ColorDepth(*bgMenu, *MenuFont3);
	ColorDepth.addLine("16");
	ColorDepth.addLine("24");
	ColorDepth.addLine("32");
	ColorDepth.addLine("Same as in Windows");
	ColorDepth.addLine(" ");
	ColorDepth.addLine("Return");

	PageInMenu Sound(*bgMenu, *MenuFont3);
	string text;

	text = "Main music - ";
	text += musicStatus ? "on" : "off";
	Sound.addLine(text);

	text = "Sound effects - ";
	text += effectsStatus ? "on" : "off";
	Sound.addLine(text);
	Sound.addLine(" ");
	Sound.addLine("Return");

	m.addPage(MainPage);
	m.addPage(Options);
	m.addPage(About);

	m.addPage(Graphics);
	m.addPage(Resolution);
	m.addPage(ColorDepth);
	m.addPage(Sound);
}

bool loadObjectData(string fileName) {
	typeDef = new int[1000];
	if(!typeDef) {
		cerr << "Could not allocate memory" << endl;
		return false;
	}

	bool loop = true;

	FILE* map = fopen(fileName.c_str(), "rb");

	if (map != NULL) {
		while (loop) {
			int endFlag = fgetc(map);

			if (endFlag == 0) {
				int index = fgetc(map);
				int value = fgetc(map);
				typeDef[index] = value;
			} else
				loop = false;	
		}
		fclose(map);
		return true;
	} else {
		cerr << "Failed loading object data" << endl;
		return false;
	}
}

int ReadData(FILE* infile) {
	int result = 0;

	int first = fgetc(infile);
	int second = fgetc(infile);
	int third = fgetc(infile);
	int fourth = fgetc(infile);

	result += first * 1000;
	result += second * 100;
	result += third * 10;
	result += fourth * 1;

	return result;
}

void FreeGraphics() {
	freeSurface(pbar);
	freeSurface(playerIdleLeft);
	freeSurface(playerIdleRight);
	for (int i = 0; i < 12; i++) {
		freeSurface(playerRunLeft[i]);
		freeSurface(playerRunRight[i]);
	}
}

int LoadMap(string fileName) {
	CameraMode = CM_FOLLOW_PLAYER;
	CollisionEngine_On = true; //pri padani sme vypli kolizie, musime ich znova povolit

	cycles = 0; //resetuj pohyb objektov

	int adr = 0;
	bool loop = true;

	FILE* map = fopen(fileName.c_str(), "rb");

	if (map != NULL) {
		int maxX = 0;
		while (loop) {
			adr++;

			int endFlag = fgetc(map);
			if (endFlag == 0) {
				objectX[adr] = ReadData(map);
				objectY[adr] = ReadData(map);
				objectT[adr] = fgetc(map);
				objData1[adr] = 0;
				objData2[adr] = 0;
				objData3[adr] = 0;
				objData4[adr] = 0;

				if (objectX[adr] > maxX)
					maxX = objectX[adr];
			} else {
				int last = 0;
				int modifier = 0;
				while (modifier != EOF) {
					last = modifier;
					modifier = fgetc(map);
				}
				modifier = last;
				loop = false;
			}
		}
		fclose(map);
		return adr;	// pocet objektov
	} else {
		cerr << "failed" << endl;
		return 0;
	}
}

bool InitVideoStuff() {
	keystate = SDL_GetKeyState(NULL);
	if (keystate[SDLK_f])
		fullscreenMode = true;

	BITDEPTH = SDL_GetVideoInfo()->vfmt->BitsPerPixel;

	if (fullscreenMode)
		myscreen = SDL_SetVideoMode(640, 460, BITDEPTH, SDL_DOUBLEBUF|SDL_FULLSCREEN|SDL_ANYFORMAT);
	else
		myscreen = SDL_SetVideoMode(640, 460, BITDEPTH, SDL_DOUBLEBUF|SDL_ANYFORMAT);

	if(!myscreen) {
		cerr << "Could not initialize video" << endl;
		return false;
	}

	return true;
}


bool SetUpEngine() {
	animFrame = new int[2000];
	objectX  = new int[2000];
	objectY  = new int[2000];
	objectT  = new int[2000];
	objData1 = new int[2000];
	objData2 = new int[2000];
	objData3 = new int[2000];
	objData4 = new int[2000];
	onPlatform = new int[2000];
	objMode  = new int[2000];

	bullets = new int[2000];
	bulletsX = new int[2000];
	bulletsY = new int[2000];
	bulletsXD = new int[2000];
	bulletsO = new bool[2000];
	bulletLife = new int[2000];

	playerRunLeft = new SDL_Surface *[12];
	playerRunRight = new SDL_Surface *[12];

	if(!animFrame || !objectX || !objectY || !objectT || !objData1 || !objData2 || !objData3 || !objData4 || !onPlatform  ||
		!objMode || !bullets || !bulletsX || !bulletsXD || !bulletsO || !bulletLife || !playerRunLeft || !playerRunRight) {
		cerr << "Could not allocate memory" << endl;
		return false;
	}

	memset (animFrame, '1', 2000);
	memset (objectX, 0, 2000);
	memset (objectY, 0, 2000);
	memset (objectT, 0, 2000);
	memset (objData1, 0, 2000);
	memset (objData2, 0, 2000);
	memset (objData3, 0, 2000);
	memset (objData4, 0, 2000);
	memset(objMode, 0, 2000);
	
	memset(bullets, 0, 2000);
	memset(bulletsX, 0, 2000);
	memset(bulletsY, 0, 2000);
	memset(bulletsXD, 0, 2000);
	memset(bulletsO, 0, 2000);
	memset(bulletLife, 0, 2000);

	return true;
}

void FreeSounds() {
	if (overworld != NULL) {
		Mix_FreeChunk(overworld);
		overworld = NULL;
	}
	if (punch != NULL) {
		Mix_FreeChunk(punch);
		punch = NULL;
	}
	if (jump != NULL) {
		Mix_FreeChunk(jump);
		jump = NULL;
	}
	if (fire != NULL) {
		Mix_FreeChunk(fire);
		fire = NULL;
	}

	Mix_CloseAudio();
}

void quitGame() {
	FreeGraphics();
	FreeSounds();

	delete playerRunLeft;
	delete playerRunRight;
	delete animFrame;
	delete objectX;
	delete objectY;
	delete objectT;
	delete objData1;
	delete objData2;
	delete objData3;
	delete objData4;
	delete onPlatform;
	delete objMode;

	delete bullets;
	delete bulletsX;
	delete bulletsY;
	delete bulletsXD;
	delete bulletsO;
	delete bulletLife;

	delete typeDef;

	delete objXL;
	delete objYL;

	SDL_ShowCursor(SDL_ENABLE);
	SDL_Quit();
}

bool SetUpGame() {
	pwr = 50;
	numberOfObjects = LoadMap(currentLevel);
	if(!numberOfObjects)
		return false;

	sPlane1 = loadImage("Data/Sprites/trees.bmp", false);
	sPlane2 = loadImage("Data/Sprites/mountain.bmp", false);
	sPlane3 = loadImage("Data/Sprites/bgclouds12.bmp", false);

	if(!sPlane1 || !sPlane2 || !sPlane3)
		return false;

	pbar = loadImage("Data/Sprites/pbar.bmp", false);

	if(!pbar)
		return false;

	if (!setTransparent(pbar, 255, 255, 255))
		return false;

	playerIdleLeft = loadImage("Data/Sprites/hercules/idle left.png", false);
	playerIdleRight = loadImage("Data/Sprites/hercules/idle right.png", false);

	if(!playerIdleLeft || !playerIdleRight)
		return false;

	for (int i = 0; i < 12; i++) {
		playerRunLeft[i] = loadImage("Data/Sprites/hercules/runleft" + toString(i + 1) + ".png", false);
		playerRunRight[i] = loadImage("Data/Sprites/hercules/run" + toString(i + 1) + ".png", false);
		if (!playerRunLeft[i] || !playerRunRight[i])
			return false;

		if(!setTransparent(playerRunLeft[i], 0, 0, 255))
			return false;
		if(!setTransparent(playerRunRight[i], 0, 0, 255))
			return false;
	}

	jumpLeft = loadImage("Data/Sprites/hercules/jump left.png", false);
	jumpRight = loadImage("Data/Sprites/hercules/jump right.png", false);
	fallLeft = loadImage("Data/Sprites/hercules/fall left.png", false);
	fallRight = loadImage("Data/Sprites/hercules/fall right.png", false);

	if(!fallLeft || !fallRight)
		return false;

	bar2 = loadImage("Data/Sprites/canvas.bmp", false);
	if (!bar2)
		return false;

	lowerSpike = loadImage("Data/Sprites/obj/14.bmp", false);

	if(!lowerSpike)
		return false;

	if(!setTransparent(lowerSpike, 0, 0, 255))
		return false;

	stairs = loadImage("Data/Sprites/obj/10.bmp", false);

	if(!stairs)
		return false;

	if (!setTransparent(stairs, 0, 0, 255))
		return false;

	if (!setTransparent(bar2, 255, 255, 255))
		return false;

	bullet = loadImage("Data/Sprites/bullet.bmp", false);

	if(!bullet)
		return false;

	if(!setTransparent(bullet, 255, 255, 255))
		return false;

	jetStick = loadImage("Data/Sprites/obj/4.bmp", false);

	if(!jetStick)
		return false;

	if(!setTransparent(jetStick, 0, 0, 255))
		return false;

	trampoline = loadImage("Data/Sprites/obj/5.bmp", false);
	ball = loadImage("Data/Sprites/obj/6.bmp", false);
	platform = loadImage("Data/Sprites/obj/7.bmp", false);

	wall = loadImage("Data/Sprites/obj/8.bmp", false);
	fliptf = loadImage("Data/Sprites/obj/9.bmp", false);
	fliptf2 = loadImage("Data/Sprites/obj/9-2.bmp", false);
	fliptf3 = loadImage("Data/Sprites/obj/9-3.bmp", false);

	upperSpike	= loadImage("Data/Sprites/obj/15.bmp", false);

	if(!trampoline || !ball || !platform || !wall || !fliptf || !fliptf2 || !fliptf3 || !upperSpike)
		return false;

	if(!setTransparent(wall, 0, 0, 255))
		return false;

	ironStick = loadImage("Data/Sprites/obj/1.bmp", false);
	if(!ironStick)
		return false;

	if(!setTransparent(ironStick, 0, 0, 255))
		return false;

	gunStick = loadImage("Data/Sprites/obj/2.bmp", false);
	if(!gunStick)
		return false;

	if(!setTransparent(gunStick, 0, 0, 255))
		return false;

	ironStick2 = loadImage("Data/Sprites/obj/1b.bmp", false);

	if(!ironStick2)
		return false;

	if(!setTransparent(ironStick2, 0, 0, 255))
		return false;

	gunStick2 = loadImage("Data/Sprites/obj/2b.bmp", false);
	if(!gunStick2)
		return false;

	if(!setTransparent(gunStick2, 0, 0, 255))
		return false;

	if(!setTransparent(upperSpike, 0, 0, 255))
		return false;

	if (!setTransparent(fliptf, 0, 0, 255))
		return false;

	if (!setTransparent(fliptf2, 0, 0, 255))
		return false;

	if (!setTransparent(fliptf3, 0, 0, 255))
		return false;

	if (!setTransparent(ball, 0, 0, 255))
		return false;

	if (!setTransparent(playerIdleLeft, 0, 0, 255))
		return false;

	if (!setTransparent(playerIdleRight, 0, 0, 255))
		return false;

	if (!setTransparent(fallLeft, 0, 0, 255))
		return false;

	if (!setTransparent(fallRight, 0, 0, 255))
		return false;

	if (!setTransparent(jumpLeft, 0, 0, 255))
		return false;

	if (!setTransparent(jumpRight, 0, 0, 255))
		return false;

	if (!setTransparent(platform, 0, 0, 255))
		return false;

	if (!setTransparent(trampoline, 0, 0, 255))
		return false;

	pos.x = 0;
	pos.y = 0;

	SDL_WM_SetCaption("Run away", NULL);

	fallSpeed = 0;
	jumping = false;

	return true;
}

bool ConfigGame() {
	if (fullscreenMode){
		myscreen = SDL_SetVideoMode(RES_X, RES_Y, BITDEPTH, SDL_DOUBLEBUF | SDL_FULLSCREEN | SDL_ANYFORMAT);
		SDL_ShowCursor(SDL_DISABLE);
	} else
		myscreen = SDL_SetVideoMode(RES_X, RES_Y, BITDEPTH, SDL_DOUBLEBUF | SDL_ANYFORMAT);

	if(!myscreen) {
		cerr << "Coult not initialize video!" << endl;
		return false;
	}

	return true;
}

bool getImagesLength() {

	objXL = new int[255];
	objYL = new int[255];

	if(!objXL || !objYL) {
		cerr << "Could not allocate memory" << endl;
		return false;
	}

	string imageDir = "Data/Sprites/obj/";
	for (int i = 1, images = 16; i < images; i++) {
			SDL_Surface *test = loadImage(imageDir + toString(i) + ".bmp", false);
			if(!test)
				return false;

			objXL[i] = test->w;
			objYL[i] = test->h;

			freeSurface(test);
	}

	return true;
}