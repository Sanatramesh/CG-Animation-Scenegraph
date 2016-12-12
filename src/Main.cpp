#include <iostream>
#include "model.h"
#include "view.h"
#include "controller.h"

using namespace std;

Model model, model1;
View view;
Scene scene;

/*
 * main: initialize glut window
 */
int main(int argc,char* argv[])
{
	if (argc != 5){
		std::cout<<"Usage: ./assignment5 <ply1 filename> <ply2 filename> <ply3 filename> <ply4 filename>"<<std::endl;
		exit(-1);
	}
	Controller controller;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(1300, 640); //Set the window size
	glutInitWindowPosition(50,50);
	//Create the window
	glutCreateWindow("Assignment 5: Animation");
	controller.run(argv);
	return 0;	
}

