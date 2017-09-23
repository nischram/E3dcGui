#ifndef __DRAWNETIMAGE_H_
#define __DRAWNETIMAGE_H_


int DrawNetImage(int x, int y, int pmAktivPhases)
{
	if (pmAktivPhases == 7)
		DrawImage("NetImage", x, y);
	else if (pmAktivPhases == 1){
		DrawImage("NetImageOff", x, y);
		put_string(x+20, y+118, "L1", GREEN);
		put_string(x+50, y+118, "L2", RED);
		put_string(x+80, y+118, "L3", RED);
	}
	else if (pmAktivPhases == 2){
		DrawImage("NetImageOff", x, y);
		put_string(x+20, y+118, "L1", RED);
		put_string(x+50, y+118, "L2", GREEN);
		put_string(x+80, y+118, "L3", RED);
	}
	else if (pmAktivPhases == 4){
		DrawImage("NetImageOff", x, y);
		put_string(x+20, y+118, "L1", RED);
		put_string(x+50, y+118, "L2", RED);
		put_string(x+80, y+118, "L3", GREEN);
	}
	else if (pmAktivPhases == 3){
		DrawImage("NetImageOff", x, y);
		put_string(x+20, y+118, "L1", GREEN);
		put_string(x+50, y+118, "L2", GREEN);
		put_string(x+80, y+118, "L3", RED);
	}
	else if (pmAktivPhases == 5){
		DrawImage("NetImageOff", x, y);
		put_string(x+20, y+118, "L1", GREEN);
		put_string(x+50, y+118, "L2", RED);
		put_string(x+80, y+118, "L3", GREEN);
	}
	else if (pmAktivPhases == 6){
		DrawImage("NetImageOff", x, y);
		put_string(x+20, y+118, "L1", RED);
		put_string(x+50, y+118, "L2", GREEN);
		put_string(x+80, y+118, "L3", GREEN);
	}
	else{
		DrawImage("NetImageOff", x, y);
		put_string(x+20, y+118, "L1", RED);
		put_string(x+50, y+118, "L2", RED);
		put_string(x+80, y+118, "L3", RED);
	}
}
#endif // __DRAWNETIMAGE_H_
