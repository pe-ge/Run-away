#ifndef DEC_H
#define DEC_H

#include "libraries.h"
#include "constants.h"

extern TTF_Font *MenuFont3;
extern TTF_Font *MenuFont4;

extern int mouse_x, mouse_y; 
extern int BITDEPTH;
extern int RES_X;
extern int RES_Y;
extern int JPOWER;
extern int LFORCE;
extern int scroller3;
extern int barlen;
extern int barleny;
extern int bar;
extern int gameOverJumped;
extern bool verticalScrolling;
extern int CameraMode;
extern int myScreenAlphaMode;
extern int camx, camy;
extern int jpf;
extern int lpx, lpy;
extern int doWasDown;
extern int TimeVelocityGained;
extern int Camera_SpeedDown;
extern int numberOfObjects;
extern bool jumping;
extern int fallSpeed;
extern bool spaceWasDown;
extern int fallRecorded;
extern int pwr;
extern int flipDown;
extern int flipEnd;
extern bool sWasDown;
extern bool aWasDown;
extern bool upWasDown;
extern bool isFacingRight;
extern bool CollisionEngine_On;

extern int plane1cx;
extern int plane2cx;
extern int plane3cx;

extern Uint32 start_ticks, ticks, ESt, ESt2;

extern int runLeftSpeed;
extern int runRightSpeed;

extern int* objectX;
extern int* objectY;
extern int* objectT;

extern int* objData1;
extern int* objData2;
extern int* objData3;
extern int* objData4;
extern int* animFrame;
extern int* onPlatform;
extern int* objMode;

extern int* bullets;
extern int* bulletsX;
extern int* bulletsY;
extern bool* bulletsO;
extern int* bulletsXD;
extern int* bulletLife;

extern int* objXL;
extern int* objYL;
extern int* typeDef;

extern Uint8  *keystate;

extern int toDoorX;
extern int toDoorY;

extern int PlayerAlpha;
extern SDL_Rect pos;
extern SDL_Rect myObjectPos; 
extern SDL_Rect plane1;
extern SDL_Rect plane2;
extern SDL_Rect plane3;
extern SDL_Rect rObj;
extern SDL_Surface *stairs;
extern SDL_Surface *bar2;
extern SDL_Surface *myscreen;
extern SDL_Surface *jetStick;
extern SDL_Surface *sPlane1;
extern SDL_Surface *sPlane2;
extern SDL_Surface *sPlane3;
extern SDL_Surface *playerIdleRight, *playerIdleLeft;
extern SDL_Surface **playerRunRight, **playerRunLeft;
extern SDL_Surface *jumpRight, *jumpLeft;
extern SDL_Surface *fallRight, *fallLeft;
extern SDL_Surface *ironStick;
extern SDL_Surface *gunStick;
extern SDL_Surface *ironStick2, *gunStick2;
extern SDL_Surface *platform;
extern SDL_Surface *trampoline;
extern SDL_Surface *ball;
extern SDL_Surface *wall;
extern SDL_Surface *fliptf, *fliptf2, *fliptf3;
extern SDL_Surface *bullet;
extern SDL_Surface *pbar;
extern SDL_Surface *upperSpike;
extern SDL_Surface *lowerSpike;

extern double ALPHA_CYCLE;
extern double ALPHA_CYCLE_2;

extern bool callMovingPtf;
extern int vertMoveEnd;

extern bool menuMode;
extern bool normalPlayerControls;
extern int adr;
extern int fall_timer;
extern int saveX, saveY;
extern int AlphaSpin;
extern int AlphaSpinMode;
extern bool fullscreenMode;
extern int cycles;

extern int direction;
extern int currentRunFrame;
extern bool running;

extern bool isPlayingMusic;
extern bool musicStatus;
extern bool effectsStatus;
extern Mix_Chunk *overworld;
extern Mix_Chunk *punch;
extern Mix_Chunk *jump;
extern Mix_Chunk *fire;

extern string firstLevel; extern int firstEndX;
extern string secondLevel; extern int secondEndX;
extern string thirdLevel;
extern string currentLevel;

extern bool DEVELOPER;
extern char cheat_str[];
extern unsigned int cheat_pos;

bool loadSounds();
bool ObjectsProgram();
bool getImagesLength();
bool SetUpEngine();
bool InitVideoStuff();
void ManageCamera();
void ManageCheats();
int LoadMap(string fileName);
int ReadData(FILE* infile);
int doComplexPlatform(int obj1x,int obj1y,int obj1ex,int obj1ey,int obj2x,int obj2y,int obj2ex,int obj2ey);
int doComplexStickmanPlatform (int obj1x,int obj1y,int obj_ljy,int obj1ex,int obj1ey,int obj2x,int obj2y,int obj2ex,int obj2ey);
bool loadObjectData(string fileName);
void killEnemy(int id);
SDL_Rect doComplexSolid(int subjx, int subjy, int subj_lx, int subjex, int subjey,int objectx,int objecty,int objectex,int objectey);
int spinAlpha();
bool programPlatform(int objectx, int objecty, int objectex, int objectey);
void programLean(int objectx, int objecty, int objectex, int objectey);
void programSolid(int objectx, int objecty, int objectex, int objectey);
void WarpPlayerTo(int location_x, int location_y);
void forceJumpPulse(int power);
int giveJumpPulse(int power);
bool SetUpGame();
bool ConfigGame();
void DrawDecor();
void ManagePlayer();
bool ManageMisc();
void ManagePowerBar();
void ManageGameOver();
void FreeSounds();
void FreeGraphics();
void quitGame();
bool initGame();
bool restartCurrentLevel();

bool isBetween(int theNumber, int inferiorBound, int superiorBound);
bool collides(int obj1x, int obj1y, int obj1xl, int obj1yl, int obj2x, int obj2y, int obj2xl,int obj2yl);

#endif