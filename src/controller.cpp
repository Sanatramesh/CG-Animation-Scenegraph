#include "controller.h"
#include "view.h"
#include "model.h"

extern Model model, model1;
extern View view;
extern Scene scene;

int Controller::flag = 1;
int Controller::mState = UP;
int Controller::oldX = -13;
int Controller::oldY = -13;
float Controller::centx = 0.0;
float Controller::centy = 0.0;
float Controller::centz = 0.0;

/*
 * keyboard: add keys for interaction
 */
void Controller::keyboard(unsigned char key, int x, int y){
	if (key == char(27) || key == 'q'){
		std::cout<<"Quiting ..."<<std::endl;
		exit(-1);
	}else if (key == 'l'){
		view.updateMovement(0., 0, 0.1);
	}else if (key == 'r'){
		view.updateMovement(0.0, 0, -0.1);
	}else if (key == '+'){
		view.scale += 0.2;
	}else if (key == '-'){
		view.scale -= 0.2;
	}else if (key == '1'){
		view.light0 = (view.light0 == 1)? 0: 1;
		std::cout<<"Light0: "<<view.light0<<std::endl;
	}else if (key == '2'){
		view.light1 = (view.light1 == 1)? 0: 1;
		std::cout<<"Light1: "<<view.light1<<std::endl;
	}else if (key == '5'){
		view.box0 = (view.box0 == 1)? 0: 1;
	}else if (key == '6'){
		view.box1 = (view.box1 == 1)? 0: 1;
	}else if (key == '3'){
		view.light2 = (view.light2 == 1)? 0: 1;
	}else if (key == '4'){
		view.light3 = (view.light3 == 1)? 0: 1;
	}else if (key == '7'){
		view.tex0 = (view.tex0 == 1)? 0: 1;
	}else if (key == '8'){
		view.tex1 = (view.tex1 == 1)? 0: 1;
	}else if (key == 'c'){
		view.cam = (view.cam == 1)? 0: 1;
		view.rotate(0,0,0,0);
	}else if (key == 'w'){
		view.updateMovement(0.01, 0, 0);
	}else if (key == 's'){
		view.updateMovement(-0.01, 0, 0);
	}else if (key == 'a'){
		view.updateMovement(0, -0.01, 0);
	}else if (key == 'd'){
		view.updateMovement(0, 0.01, 0);
	}	else if  (key == 'b'){
		view.update(1, 0);
	}else if  (key == 't'){
		view.update(0, 1);
	}
	glutPostRedisplay();
}

/*
 * glutMotion: add mouse drag for interaction
 */
void Controller::glutMotion(int x, int y)
{
	if (mState == DOWN)
    {
		view.rotate(oldX, oldY, x, y);
		glutPostRedisplay();
    }
    oldX = x;
    oldY = y;
}

/*
 * glutMouse: add mouse click for interaction
 */
void Controller::glutMouse(int button, int state, int x, int y)
{
	float dist = 0.0;
	float px = 0, py = 0;
    if(state == GLUT_DOWN)
    {
        switch(button)
        {
            case GLUT_LEFT_BUTTON:
				oldX = x;
                oldY = y;
                break;
            case GLUT_RIGHT_BUTTON:
				mState = DOWN;
                oldX = x;
                oldY = y;
            break;
        }
    } else if (state == GLUT_UP){
        mState = UP;
        switch(button)
        {
            case GLUT_LEFT_BUTTON:
				oldX = x;
                oldY = y;
                break;
            case GLUT_RIGHT_BUTTON:
				view.rotate(0, 0, 0, 0);
				break;
        }        
        glutPostRedisplay();
	}
}

/*
 * SpecialKeys: add special keys for interaction
 */
void Controller::SpecialKeys(int key, int x, int y){
	if (key == GLUT_KEY_LEFT){
		view.transx-=0.1;
	}
	else if (key == GLUT_KEY_UP){
		view.transy+=0.1;
	}
	else if (key == GLUT_KEY_RIGHT){
		view.transx+=0.1;
	}
	else if (key == GLUT_KEY_DOWN){
		view.transy-=0.1;
	}
	glutPostRedisplay();
}

/*
 * run: initialize scene, glut keyboard, mouse, etc and start Mainloop
 */
void Controller::run(char** argv){
	
	scene.init(argv);
	scene.computeModelNormals();
		
	view.initRendering(); //Initialize rendering
	//Set handler function for drawing
	glutDisplayFunc(view.draw);
	//Set handler function for window resize
	glutReshapeFunc(view.handleResize);
	//Set handler function for keypress
	glutKeyboardFunc(keyboard);
	//Set handler function for Special keypress
	glutSpecialFunc(SpecialKeys);
	glutMotionFunc(glutMotion);
	glutMouseFunc(glutMouse);
	glutMainLoop(); //Start the main loop.	
}
