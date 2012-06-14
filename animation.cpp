#include "processObjects.h"

//dalsi objekt v subore sa ma hybat
void processAnimation(int id) {
	cycles += 40;
	if (cycles > TOTALCYCLES)
		cycles = 0;

	if (cycles < TOTALCYCLES / 2) {
		int distX, distY;
		distX = objectX[id + 2] - objectX[id + 3];
		distY = objectY[id + 2] - objectY[id + 3];
		objectX[id + 1] = objectX[id + 2] - (objectX[id + 2] - objectX[id + 3]) * (cycles / (TOTALCYCLES / 2));
		objectY[id + 1] = objectY[id + 2] - (objectY[id + 2] - objectY[id + 3]) * (cycles / (TOTALCYCLES / 2));
	} else {
		int distX,distY;
		distX = objectX[id + 3] - objectX[id + 2];
		distY = objectY[id + 3] - objectY[id + 2];
		objectX[id + 1] = objectX[id + 3] + (objectX[id + 2] - objectX[id + 3]) * ((cycles - (TOTALCYCLES / 2)) / (TOTALCYCLES / 2));
		objectY[id + 1] = objectY[id + 3] + (objectY[id + 2] - objectY[id + 3]) * ((cycles - (TOTALCYCLES / 2)) / (TOTALCYCLES / 2));
	}
}