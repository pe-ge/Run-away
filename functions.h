#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "libraries.h"

//vseobecne funkcie ktore sa nespajaju priamo s hrou

SDL_Surface *loadImage(string filename, bool alpha);
void putPixel(SDL_Surface *surface, long x, long y, Uint32 pixel);
void freeSurface(SDL_Surface *theSurf);
string toString(int number);
bool setTransparent(SDL_Surface *surface, long r, long g, long b);
bool isBetween(int theNumber, int inferiorBound, int superiorBound);
bool collides(int obj1x, int obj1y, int obj1xl, int obj1yl, int obj2x, int obj2y, int obj2xl, int obj2yl);

#endif 