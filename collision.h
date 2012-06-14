#ifndef COLLISION_H
#define COLLISION_H

#include "dec.h"

void programSolid(int objx1, int objy1, int objx2, int objy2);
SDL_Rect doComplexSolid(int subjx, int subjy, int subj_lx, int subjex, int subjey, int objectx, int objecty, int objectex, int objectey);
int doComplexStickmanPlatform (int obj1x, int obj1y, int obj_ljy, int obj1ex, int obj1ey, int obj2x, int obj2y, int obj2ex, int obj2ey);
int doComplexPlatform(int obj1x, int obj1y, int obj1ex, int obj1ey, int obj2x, int obj2y, int obj2ex, int obj2ey);
void programLean(int objectx, int objecty, int objectex, int objectey);
bool programPlatform(int objx1, int objy1, int objx2, int objy2);

#endif