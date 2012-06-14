#include "functions.h"

SDL_Surface *loadImage(string filename, bool alpha){
	SDL_Surface *tmp = IMG_Load(filename.c_str());
	if (tmp == NULL){
		cout << "ERROR: " << SDL_GetError() << endl;
		getchar();
		return NULL;
	}

 	SDL_Surface *ret = (alpha) ? SDL_DisplayFormatAlpha(tmp) : SDL_DisplayFormat(tmp);
	if (ret == NULL){
		cout << "ERROR: " << SDL_GetError() << endl;
		getchar();
		return NULL;
	}

	SDL_FreeSurface(tmp);
	return ret;
}

void putPixel(SDL_Surface *surface, long x, long y, Uint32 pixel) {
	long bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16 *)p = pixel;
			break;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			} else {
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;

		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}

void freeSurface(SDL_Surface *theSurf) {
	if (theSurf != NULL)
		SDL_FreeSurface(theSurf);
}

string toString(int number) {
   stringstream ss;
   ss << number;
   return ss.str();
}


bool setTransparent(SDL_Surface *surface, long r, long g, long b) {
	Uint32 color = SDL_MapRGB(surface->format, r, g, b);
	if(SDL_SetColorKey(surface, SDL_SRCCOLORKEY | SDL_RLEACCEL, color) < 0) {
		cerr << "Could not set transparency" << endl;
		return false;
	}

	return true;
}

bool isBetween(int theNumber, int inferiorBound, int superiorBound) {
	if(theNumber > inferiorBound && theNumber < superiorBound)
		return true;
	else
		return false;
}

bool collides(int obj1x, int obj1y, int obj1xl, int obj1yl, int obj2x, int obj2y, int obj2xl, int obj2yl) {
	if(isBetween(obj1x, obj2x, obj2x + obj2xl) && isBetween(obj1y, obj2y, obj2y + obj2yl))
		return true;

	if(isBetween(obj1x + obj1xl, obj2x, obj2x + obj2xl) && isBetween(obj1y, obj2y, obj2y + obj2yl))
		return true;

	if(isBetween(obj1x, obj2x, obj2x + obj2xl) && isBetween(obj1y + obj1yl, obj2y, obj2y + obj2yl))
		return true;

	if(isBetween(obj1x + obj1xl, obj2x, obj2x + obj2xl) && isBetween(obj1y + obj1yl, obj2y, obj2y + obj2yl))
		return true;

	if(isBetween(obj2x, obj1x, obj1x + obj1xl) && isBetween(obj2y, obj1y, obj1y + obj1yl))
		return true;

	if(isBetween(obj2x + obj2xl, obj1x, obj1x + obj1xl) && isBetween(obj2y, obj1y, obj1y + obj1yl))
		return true;

	if(isBetween(obj2x, obj1x, obj1x + obj1xl) && isBetween(obj2y + obj2yl, obj1y, obj1y + obj1yl))
		return true;

	if(isBetween(obj2x + obj2xl, obj1x, obj1x + obj1xl) && isBetween(obj2y + obj2yl, obj1y, obj1y + obj1yl))
		return true;

	return false;
}