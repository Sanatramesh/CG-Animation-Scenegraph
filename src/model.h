#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <SOIL.h>

class Point{
	int n;
	float x, y, z;
	float normx, normy, normz;
	float r, s;
public:
	Point();
	Point(float x_coor, float y_coor, float z_coor);
	float getX();
	void setX(float x_coor);
	float getY();
	void setY(float y_coor);
	float getZ();
	void setZ(float z_coor);
	float getNormX();
	float getNormY();
	float getNormZ();
	float getTexR();
	float getTexS();
	float setTexture(float nr, float ns);
	void print();
	void normNormal();
	void textureMapCylinder();
	void textureMapSphere();
	void setNormal(float normx, float normy, float normz);
	void addNormal(float normx, float normy, float normz);
};

class Polygon{
	int no;
	std::vector<Point*> vertices;
	float normx, normy, normz;
public:
	Polygon();
	void addVertex(Point *p);
	Point* getPointAt(int i);
	void printVertices();
	void drawPolygon();
	void computeNormal();
	void normalize();
	void computeTexSphere();
	void computeTexCylinder();
};

class Model{
	std::vector<Point*> points; // List of input points
	std::vector<Polygon*> polygons; // List of polygons
	float minx, miny, minz;
	float maxx, maxy, maxz;
	int width, height;
	unsigned char *image;
	GLuint tex;
	int flg_nx, flg_ts;
public:
	Model();
	Model(char *fname);
	void render();
	void setTexture();
	void computeNormals();
	void drawBoundingBox();
	void textureMapSphere();
	void textureMapCylinder();
	virtual void drawObject();
	void readPlyFile(char* fname);
	void readObjFile(char* fname);
	void readPlyFileAsIs(char* fname);
	void readTexture(char* texname);
	void setVertices(std::vector<Point*> p);
	virtual void setDirection(GLfloat *pos);
	void setPolygons(std::vector<Polygon*> polys);
	virtual void nextPosition(float *transm, float *rotsm, float* scle);
};
class Light: public Model{
public:
	Light();
	void drawObject();
	void setDirection(GLfloat *pos);
	void nextPosition(float *transm, float *rotsm, float* scle);
};
class Tankhead: public Model{
	int time;
public:
	Tankhead(char *fname);
	void drawObject();
	//~ void setDirection(GLfloat *pos);
	void nextPosition(float *transm, float *rotsm, float* scle);
};
class Tankbase: public Model{
	int time;
public:
	Tankbase(char *fname);
	void drawObject();
	//~ void setDirection(GLfloat *pos);
	void nextPosition(float *transm, float *rotsm, float* scle);
};
class Spaceship: public Model{
	int time;
public:
	Spaceship(char *fname);
	void drawObject();
	//~ void setDirection(GLfloat *pos);
	void nextPosition(float *transm, float *rotsm, float* scle);
};
class Jet: public Model{
	int time;
public:
	Jet(char *fname);
	void drawObject();
	//~ void setDirection(GLfloat *pos);
	void nextPosition(float *transm, float *rotsm, float* scle);
};
class Ship: public Model{
	int time;
public:
	Ship(char *fname);
	void drawObject();
	//~ void setDirection(GLfloat *pos);
	void nextPosition(float *transm, float *rotsm, float* scle);
};
class SceneGraph{
	Model *model;
	std::vector<SceneGraph*> scene_models;
	float trans_matrix[3];
	float rot_matrix[3];
	float scale;
public:
	SceneGraph();
	SceneGraph(Model *m);
	SceneGraph(char* fname);
	SceneGraph(float* trans_mat);
	SceneGraph(Model *m, float* trans_mat);
	SceneGraph(char* fname, float* trans_mat);
	SceneGraph(char* fname, char *tex, float* trans_mat);
	SceneGraph* getNode(int i);
	void getPosition(GLfloat *pos);
	void addTexture(char *tex);
	void drawModel();
	void drawGraph();
	Model* getModel();
	void setTransitions();
	void setModel(Model *m);
	void texSphericalMap();
	void texCylindricalMap();
	void computeModelNormal();
	SceneGraph* popSceneNode();
	void updateTransitions(float *pos);
	void appendModelNode(SceneGraph* sm);
};
class Scene{
	std::vector<int> lights;
	std::vector<SceneGraph*> scene_graph;
	float eye[4];
	float transmat[6];
	int time, changecam;
public:
	Scene();
	void drawAxis();
	void drawScene();
	void updateScene();
	void init(char** argv);
	void update(int b, int t);
	void updateMovement(float *t);
	void computeModelNormals();
	void addModel(SceneGraph* sg);
	void readObjFile(char* fname);
	void drawFloor(float size, int tex);
	void multMatrix(float *e, float *rot);
	void updateLightState(int idx, int flag);
	void drawBackground(float size, int tex);
	void setLightPosition( int idx, float *pos);
	void setSpotDirection( int idx, float *dir);
	void setCamera(float *trans, float *rotmat, int cam);
	void setSceneGraph(std::vector<SceneGraph*> sg);
	void addLight(float *amb, float *dif, float *spec, float cutoff, float exp);
};
#endif
