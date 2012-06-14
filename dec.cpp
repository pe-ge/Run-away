#include "dec.h"

int BITDEPTH = 32;
int RES_X = 640;
int RES_Y = 480;

int mouse_x = 0, mouse_y = 0;

TTF_Font *MenuFont3; //font pouzity v hre
TTF_Font *MenuFont4; //font pouzity v hre

int JPOWER = DEF_JPOWER;

int scroller3;

int barlen = 242;
int barleny = 19;

int bar;

int gameOverJumped;
bool verticalScrolling = true; //ci sa hybe obrazovka za hracom na osi y

int CameraMode = CM_FOLLOW_PLAYER;

int myScreenAlphaMode = 1;

int camx, camy; //pozicia kamery
int jpf; //rychlost skakania
int lpx, lpy; //na docasne ulozenie pozicii x, y
int doWasDown;
int TimeVelocityGained;

int Camera_SpeedDown;
int numberOfObjects; //pocet objektov
bool jumping; //ci skaceme
int fallSpeed; //rychlost padu, pri skakani a rovinke skace na 0,1; ako fall_timer len ma hornu hranicu 17
bool spaceWasDown; //ci bol stlaceny medzernik
int fallRecorded;
int pwr; //zivot
int flipDown = 0;
int flipEnd;
bool sWasDown; //ci bolo stlacene s
bool aWasDown; //ci bolo stlacene s
bool upWasDown; //ci bola stlacena sipka hore

bool isFacingRight = false;

bool CollisionEngine_On = true; //zapnutie detekcie kolizii

//pozadie - jednotlive vrstvy
int plane1cx;
int plane2cx;
int plane3cx;

Uint32 start_ticks, ticks, ESt, ESt2;

int runLeftSpeed;
int runRightSpeed;

int* objectX = NULL;
int* objectY = NULL;
int* objectT   = NULL;

int* objData1   = NULL; //posledna pozicia pri skoku
int* objData2   = NULL; //rychlost padania
int* objData3   = NULL; //posledna x-ova pozicia
int* objData4   = NULL; //rychlost
int* animFrame = NULL; //indexy do animations[];
int* onPlatform = NULL;

int* objMode   = NULL;

int* bullets    = NULL; //do bullets[id] sa uklada pocet nabojov, ktore object[id] vystrelilo
int* bulletsX   = NULL; //x-ova pozicia naboja
int* bulletsY   = NULL; //y-ova pozicia naboja
bool* bulletsO   = NULL; //postupnost nabojov, kde posledny vystreleny naboj ma hodnotu 0, ostatne 255
int* bulletsXD  = NULL; //rychlost nabojoa
int* bulletLife  = NULL; //zivot naboja

// type defs
int* objXL     = NULL; //sirky objektov v data/sprites/obj
int* objYL     = NULL; //vysky objektov
int* typeDef   = NULL;

Uint8  *keystate;

int toDoorX;
int toDoorY;

int PlayerAlpha = 255;
SDL_Rect pos; //hracova pozicia
SDL_Rect myObjectPos; 
SDL_Rect plane1; //pozicia pozadia na oblohe
SDL_Rect plane2;
SDL_Rect plane3;
SDL_Rect rObj; //rect object, na roznych mestiach sa pouziva
SDL_Surface *stairs;
SDL_Surface *bar2;
SDL_Surface *myscreen; //hlavna obrazovka
SDL_Surface *jetStick; //stick s jetpackom
SDL_Surface *sPlane1; //lesy v pozadi
SDL_Surface *sPlane2;  //hory v pozadi
SDL_Surface *sPlane3; //mraky
SDL_Surface *playerIdleRight, *playerIdleLeft; //hrac
SDL_Surface **playerRunRight, **playerRunLeft; //beziaci hrac
SDL_Surface *jumpRight, *jumpLeft; //ked skace
SDL_Surface *fallRight, *fallLeft; //ked pada
SDL_Surface *ironStick; //nepriatel
SDL_Surface *gunStick; //nepriatel so zbranou
SDL_Surface *ironStick2, *gunStick2; //otoceny
SDL_Surface *platform; //mechanicka plosina
SDL_Surface *trampoline; //5.bmp
SDL_Surface *ball; //6.bmp
SDL_Surface *wall; //8.bmp
SDL_Surface *fliptf, *fliptf2, *fliptf3; //prepadujuca sa plosina 9.bmp
SDL_Surface *bullet;
SDL_Surface *pbar; //stavovy riadok zivota
SDL_Surface *upperSpike; //pichlace zdola nahor
SDL_Surface *lowerSpike; //pichlace zhora nadol

double ALPHA_CYCLE = 0;
double ALPHA_CYCLE_2 = 0;

bool callMovingPtf = false;
int vertMoveEnd = 0;

bool menuMode = false;
bool normalPlayerControls = true;
int adr;
int fall_timer; //cas pri padani, kazdy frame sa zvysi
int saveX,saveY; //ulozenie hracovej pozicie pomocou g
int AlphaSpin = 255;
int AlphaSpinMode = 1;
bool fullscreenMode = false;
int cycles;
int direction = RIGHT; //kde je otoceny
int currentRunFrame = 0; //aktualny sprite pri behani
bool running = false;

bool isPlayingMusic = false;
bool musicStatus = true;
bool effectsStatus = true;
Mix_Chunk *overworld = NULL;
Mix_Chunk *punch = NULL;
Mix_Chunk *jump = NULL;
Mix_Chunk *fire = NULL;

string firstLevel = "Data\\Maps\\first.map"; int firstEndX = 7500;
string secondLevel = "Data\\Maps\\second.map"; int secondEndX = 4900;
string thirdLevel = "Data\\Maps\\third.map";
string currentLevel = firstLevel;

bool DEVELOPER = false;
char cheat_str[] = "godmode";
unsigned int cheat_pos = 0;