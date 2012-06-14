#ifndef INIT_H
#define INIT_H

#include "dec.h"
#include "CMenu.h"

bool initGame();
bool loadSounds();
void createMenu(CompleteMenu &m);
bool loadObjectData(string fileName);
int ReadData(FILE* infile);
void FreeGraphics();
int LoadMap(string fileName);
bool InitVideoStuff();
bool SetUpEngine();
void FreeSounds();
void quitGame();
bool SetUpGame();
void SetupMenu();
bool ConfigGame();
bool getImagesLength();

#endif