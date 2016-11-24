#ifndef __DRAWNETIMAGE_H_
#define __DRAWNETIMAGE_H_


int DrawNetImage(int pmAktivPhases)
{
	if (pmAktivPhases == 7)
		DrawImage("NetImage", 640, 50);
	else if (pmAktivPhases == 1){
		DrawImage("NetImageOff", 640, 50);
		put_string(660, 168, "L1", GREEN);
		put_string(690, 168, "L2", RED);
		put_string(720, 168, "L3", RED);
	}
	else if (pmAktivPhases == 2){
		DrawImage("NetImageOff", 640, 50);
		put_string(660, 168, "L1", RED);
		put_string(690, 168, "L2", GREEN);
		put_string(720, 168, "L3", RED);
	}
	else if (pmAktivPhases == 4){
		DrawImage("NetImageOff", 640, 50);
		put_string(660, 168, "L1", RED);
		put_string(690, 168, "L2", RED);
		put_string(720, 168, "L3", GREEN);
	}
	else if (pmAktivPhases == 3){
		DrawImage("NetImageOff", 640, 50);
		put_string(660, 168, "L1", GREEN);
		put_string(690, 168, "L2", GREEN);
		put_string(720, 168, "L3", RED);
	}
	else if (pmAktivPhases == 5){
		DrawImage("NetImageOff", 640, 50);
		put_string(660, 168, "L1", GREEN);
		put_string(690, 168, "L2", RED);
		put_string(720, 168, "L3", GREEN);
	}
	else if (pmAktivPhases == 6){
		DrawImage("NetImageOff", 640, 50);
		put_string(660, 168, "L1", RED);
		put_string(690, 168, "L2", GREEN);
		put_string(720, 168, "L3", GREEN);
	}
	else{
		DrawImage("NetImageOff", 640, 50);
		put_string(660, 168, "L1", RED);
		put_string(690, 168, "L2", RED);
		put_string(720, 168, "L3", RED);
	}
}
#endif // __DRAWNETIMAGE_H_
