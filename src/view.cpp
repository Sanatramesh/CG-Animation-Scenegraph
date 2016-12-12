#include "view.h"
#include "model.h"

int View::h = 0;
int View::w = 0;

extern Model model, model1;
extern Scene scene;
float View::eye[4] ={ .05, 2, 5, 1};
float View::transx = 0.0;
float View::transy = 0.0;
float View::transz = 0.0;
float View::scale = 1;
float View::angle = 0.0;
int View::light0 = 0;
int View::light1 = 1;
int View::light2 = 1;
int View::light3 = 0;
int View::box0 = 1;
int View::box1 = 1;
int View::tex0 = 1;
int View::tex1 = 1;
int View::cam = 0;
GLuint View::tex[] = {0, 0, 0};
Trackball View::trackball;
unsigned char* image;
int width, height;

/*
 * initRendering: Setting up glut window, view, lights and textures
 */
void View::initRendering() {
	w = glutGet( GLUT_WINDOW_WIDTH );
	h = glutGet( GLUT_WINDOW_HEIGHT );
	glViewport(0, 0, w, h);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	
	GLfloat spot_direction[] = { 0.0, 0.0, 0.0 };
	GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	
	// Parameters for LIGHT1
	GLfloat light_ambient[] = { .0, .0, .0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	
	scene.addLight(light_ambient, light_diffuse, light_specular, 15.0f, 100.0f); 
	
	// Paramters for LIGHT2
	GLfloat light_ambient1[] = { .0, .0, .0, 1.0 };
	GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };
	
	scene.addLight(light_ambient1, light_diffuse1, light_specular1, 15.0f, 100.0f);
	
	// Parameters for LIGHT3
	GLfloat light_ambient3[] = { 1, 0, 1.0, 1.0 };
	GLfloat light_diffuse3[] = { 0.0, 0.0, .0, 1.0 };
	GLfloat light_specular3[] = { 1.0, 1.0, 1.0, 1.0 };
	
	scene.addLight(light_ambient3, light_diffuse3, light_specular3, 30.0f, 100.0f);
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS,mat_shininess);
	
	// Texture initialization
	glGenTextures(1, &tex[0]);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	// Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// Texture Load
	image = SOIL_load_image("../bin/water3.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
              GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	
	// Texture initialization
	glGenTextures(1, &tex[1]);
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	// Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// Texture Load
	image = SOIL_load_image("../bin/sky2.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
              GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	
	glEnable(GL_DEPTH_TEST);	
	glClearColor(0.0, 0.0, .0, 0.0);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();    
	gluPerspective( 60.0, w / h, 0.01, 40.0 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
 * handleResize: Setting up glut window and view after window resize
 */
void View::handleResize(int wt, int ht){
	if (ht  == 0)
		ht = 1;
	
	w = wt;
	h = ht;
	glViewport(0,0, wt, ht);
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective( 45.0, w /(float)h, 0.1, 150.0 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
 * rotate: rotate the object using trackball
 */
void View::rotate(float oldx, float oldy, float x, float y){
	trackball.rotate((2.0*oldx - w)/(float)w,
					 (h - 2.0*oldy)/(float)h,
					 (2.0*x - w)/(float)w,
					 (h - 2.0*y)/(float)h);
}

/*
 * updateMovement: apply transformation on the complete scene
 */
void View::updateMovement(float x, float y, float angle){
	float transm[] = {x, 0,y, 0, angle, 0, 1};
	scene.updateMovement(&transm[0]);
}

/*
 * update: update the scene
 */
void View::update(int b, int t){
	scene.update(b, t);
}

/*
 * draw: Render the axis, model and bounding box
 */
void View::draw(){
	GLfloat position[] = { 2, 4.0, -3, 1.0 };
	GLfloat position1[] = { -2, 4, -3, 1.0 };
	float rotation_transform[4][4];
	GLfloat spotlight1[] = { -0.2, -4, 0.5};
	GLfloat spotlight[] = { 0, -4.0, 0};		
	GLfloat amb[] = { 0, 10.0, -10, 1.0 };
	float tran[] = {transx, transy, transz, angle, angle, angle, scale};
	eye[0] = .05+transx;
	eye[1] = 5+transy;
	eye[2] = 10+transz-scale;
	eye[3] = 1;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	trackball.rotationMatrix( rotation_transform );
	
	scene.setCamera(tran, &rotation_transform[0][0], cam);
	
	scene.updateLightState(0, light0);
	scene.updateLightState(1, light1);
	scene.updateLightState(2, light2);
	scene.updateLightState(3, light3);

	GLfloat spotlight2[] = {  transx-eye[0], transy-eye[1], transz-eye[2]};
	
		
	// Drawing Axises
	scene.drawAxis();
	
	//Drawing two light sources
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(position[0], position[1], position[2]);
	glutWireSphere(0.025, 10, 10);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslatef(position1[0], position1[1], position1[2]);
	glutWireSphere(0.025, 10, 10);
	glPopMatrix();
	
	// Setting Light Position
	scene.setLightPosition(0, amb);
	scene.setLightPosition(1, position);
	scene.setSpotDirection(1, spotlight);
	scene.setLightPosition(2, position1);
	scene.setSpotDirection(2, spotlight1);
	scene.setLightPosition(3, eye);
	scene.setSpotDirection(3, spotlight2);
	
    // Texture Mapping the Floor
    //~ scene.drawBackground(50, tex[1]);
	scene.drawFloor(50, tex[0]);
	scene.drawScene();
	glDisable (GL_LIGHTING);
	glutSwapBuffers();
	usleep(50000);
	glutPostRedisplay();
}

/*
 * multMatrix: compute matrix multiplication of e and rot
 */
void View::multMatrix(float *e, float *rot ){
	int i, j;
	float c[4];
	for (i=0; i<4;i++){
		c[i] = 0.0;
		for (j=0; j<4;j++){
			c[i] += e[j]*rot[4*j+i];
		}
	}
	for (i=0; i<3;i++){
		e[i] = c[i];
	}
}
