#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "view.h"
#include "model.h"

class Controller{
	static int h, w;
	enum {UP = 1, DOWN, };
	static int flag;
	static int mState;
	static int oldX, oldY;
	static float centx, centy, centz;
public:
	void run(char**);
	static void SpecialKeys(int key, int x, int y);
	static void glutMotion(int x, int y);
	static void glutMouse(int button, int state, int x, int y);
	static void keyboard(unsigned char key, int x, int y);
};
#endif
