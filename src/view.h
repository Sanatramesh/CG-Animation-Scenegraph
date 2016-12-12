#ifndef VIEW_H
#define VIEW_H

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <SOIL.h>
#include <unistd.h>
#include "FastTrackball.h"

class View{
	static int w, h;
	static Trackball trackball;
	static float eye[4];
public:
	static float transx, transy, transz;
	static float angle, scale;
	static int light0, light1, light2, light3, box0, box1;
	static int tex0, tex1, cam;
	static GLuint tex[3];
	static void draw();
	static void update(int b, int t);
	static void updateMovement(float x, float y, float angle);
	static void initRendering();
	static void handleResize(int w, int h);
	static void rotate(float x1, float y1, float x2, float y2);
	static void multMatrix(float *e, float *rot );
};
#endif
