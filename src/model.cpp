#include "model.h"
 
int box = 0;
int texture = 1;

/*
 * Point class - represents each vertex in the mesh of the 3D object
 */
// Point constructor
Point::Point(){
	x = 0.0;
	y = 0.0;
	z = 0.0;
	n = 0;
	r = 0.0;
	s = 0.0;
	normx = 0.0;
	normy = 0.0;
	normz = 0.0;
}
// Point constructor
Point::Point(float x_coor, float y_coor, float z_coor){
	x = x_coor;
	y = y_coor;
	z = z_coor;
	n = 0;
	r = 0.0;
	s = 0.0;
	normx = 0.0;
	normy = 0.0;
	normz = 0.0;
}

float Point::getX(){
	return x;
}

void Point::setX(float x_coor){
	x = x_coor;
}

float Point::getY(){
	return y;
}
void Point::setY(float y_coor){
	y = y_coor;
}
float Point::getZ(){
	return z;
}
void Point::setZ(float z_coor){
	z = z_coor;
}
float Point::getNormX(){
	return normx;
}

float Point::getNormY(){
	return normy;
}
float Point::getTexR(){
	return r;
}
float Point::getTexS(){
	return s;
}
float Point::getNormZ(){
	return normz;
}
void Point::print(){
	std::cout<<x<<" "<<y<<" "<<z<<std::endl;
}
void Point::setNormal(float nx, float ny, float nz){
	normx = nx;
	normy = ny;
	normz = nz;
}
void Point::addNormal(float nx, float ny, float nz){
	normx += nx;
	normy += ny;
	normz += nz;
	n += 1;
}
void Point::normNormal(){ // Normalize the normal of this polygon 
	if (n == -1)
		return ;
	normx /= n; normy /= n; normz /= n;
	float norm = sqrt(normx*normx + normy*normy + normz*normz);
	normx /= norm; normy /= norm; normz /= norm;
	n = -1;
}
float Point::setTexture(float nr, float ns){
	r = nr;
	s = ns;
}
// compute cylindrical mapping for Point(x, y, z)
void Point::textureMapCylinder(){	
	s = y;
	r = (atan2f(z, x)  + 3.14)/(2*3.14);
}
// compute spherical mapping for Point(x, y, z)
void Point::textureMapSphere(){
	r = (atan2f(z, x)  + 3.14)/(2*3.14);
	s = (atan2f(z, y*sin(2*3.14*r)))/(2*3.14);
}

/*
 * Polygon class - represents each polygon of the 3D object
 */
// Polygon constructor
Polygon::Polygon(){
	no = -1;
}
void Polygon::normalize(){
	int i;
	for (i=0;i<vertices.size();i++)
		vertices[i]->normNormal();
}
void Polygon::addVertex(Point* p){
	vertices.push_back(p);
}
Point* Polygon::getPointAt(int i){
	if (no != -1 && no != i)
		return vertices[i];
	return nullptr;
}
void Polygon::printVertices(){
	int i;
	for (i=0;i<vertices.size();i++)
		vertices[i]->print();
}
void Polygon::drawPolygon(){
	if (vertices.size() == 2)
		glBegin(GL_LINES);
	else if (vertices.size() == 3)
		glBegin(GL_TRIANGLES);
	else
		glBegin(GL_QUADS);
	glColor4f(1., 1., 1., 0.);
	for (int i=0;i < vertices.size(); i++){
		glNormal3f(vertices[i]->getNormX(), vertices[i]->getNormY(), vertices[i]->getNormZ());
		glTexCoord2f(vertices[i]->getTexR(), -vertices[i]->getTexS());
		glVertex3f(vertices[i]->getX(), vertices[i]->getY(), vertices[i]->getZ());
	}
	glEnd();
}
// compute texture coordinates (r,s) for each vertex of the polygon using spherical mapping
void Polygon::computeTexSphere(){
	int i=0;
	for (int i=0;i < vertices.size(); i++)
		vertices[i]->textureMapSphere();
}
// compute texture coordinates (r,s) for each vertex of the polygon using cylinderical mapping
void Polygon::computeTexCylinder(){
	int i=0;
	for (int i=0;i < vertices.size(); i++)
		vertices[i]->textureMapCylinder();
}
// compute normal to this polygon
void Polygon::computeNormal(){
	float n = 0;
	float v1x, v1y, v1z, v2x, v2y, v2z;
	if (vertices.size() >= 3){		
		v1x = vertices[1]->getX() - vertices[0]->getX();
		v1y = vertices[1]->getY() - vertices[0]->getY();
		v1z = vertices[1]->getZ() - vertices[0]->getZ();
		
		v2x = vertices[2]->getX() - vertices[0]->getX();
		v2y = vertices[2]->getY() - vertices[0]->getY();
		v2z = vertices[2]->getZ() - vertices[0]->getZ();
		
		normx = v1y*v2z - v1z*v2y;
		normy = v1z*v2x - v1x*v2z;
		normz = v1x*v2y - v1y*v2x;
		n = sqrt(normx*normx + normy*normy + normz*normz);
	}
	for (int i=0;i<vertices.size(); i++)
		vertices[i]->addNormal(normx, normy, normz);
}

/*
 * Model class - represents the 3D object
 */
// Model constructor
Model::Model(){
	minx = 9999.0; miny = 9999.0; minz = 9999.0;
	maxx = -9999.0; maxy = -9999.0; maxz = -9999.0;
	flg_nx = 0; flg_ts = 0;
}
// Model constructor
Model::Model(char *fname){
	minx = 999999.0; miny = 999999.0; minz = 9999999.0;
	maxx = -9999.0; maxy = -9999.0; maxz = -9999.0;
	flg_nx = 0; flg_ts = 0;
	readPlyFileAsIs(fname);
}
// Render the model and axis 
void Model::render(){
	drawObject();
	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3f (1, 0, 0);
	glVertex3f(0, 0, 0);
	glEnd();
}
// Compute texture mapping of the model using cylindircal mapping 
void Model::textureMapCylinder(){	
	int i;
	std::cout<<"Texture: "<<flg_ts<<std::endl;
	if (flg_ts)
		return;
	for (i=0; i < polygons.size(); i++){
		polygons[i]->computeTexCylinder();
	}
}
// Compute texture mapping of the model using spherical mapping 
void Model::textureMapSphere(){
	int i;
	std::cout<<"Texture: "<<flg_ts<<std::endl;
	if (flg_ts)
		return;
	for (i=0; i < polygons.size(); i++){
		polygons[i]->computeTexSphere();
	}
}
// read texture from a file
void Model::readTexture(char* texname){
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	// Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Texture Load
	image = SOIL_load_image(texname, &width, &height, 0, SOIL_LOAD_RGB);
	std::cout<<"Texture Size: "<<width<<" "<<height<<std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
              GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
}
void Model::setTexture(){
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, tex);
}
// Render Object
void Model::drawObject(){
	long long i;
	for(i=0;i < polygons.size();i++)
		polygons[i]->drawPolygon();
}
// Read object from ply file
void Model::readPlyFile(char *filename){
	std::cout<<filename<<std::endl;
	std::ifstream ifile;
	ifile.open(filename);
	std::string s;
	long int num_vertex, num_faces; long long i;
	float x, y, z, nx, ny, nz, t, r;
	int co_in = 0, norm = 0, map = 0, bx = 0;
	int vertex_idx, j, sides = 0;
	Polygon* polygon;
	Point *p;
	while(ifile>>s){
		if (s.compare("vertex") == 0){
			ifile>>s;
			num_vertex = std::stoi(s);
			std::cout<<num_vertex<<std::endl;
		}else if (s.compare("face") == 0){
			ifile>>s;
			num_faces = std::stoi(s);
			std::cout<<num_faces<<std::endl;
		}else if (s.compare("confidence") == 0 || s.compare("intensity") == 0){
			co_in = 1;
		}else if (s.compare("nx") == 0){
			norm = 1;
		}else if (s.compare("t") == 0){
			map = 1;
		}else if (s.compare("bx") == 0){
			bx = 1;
		}else if (s.compare("end_header") == 0){
			for ( i=0; i < num_vertex; i++){
				ifile>>s;
				x = std::stof(s);
				ifile>>s;
				y = std::stof(s);
				ifile>>s;
				z = std::stof(s);
				p = new Point(x, y, z);
				points.push_back(p);
				if (minx > x)
					minx = x;
				if (miny > y)
					miny = y;
				if (minz > z)
					minz = z;
				if (maxx < x)
					maxx = x;
				if (maxy < y)
					maxy = y;
				if (maxz < z)
					maxz = z;
				if (co_in){
					ifile>>s;
					ifile>>s;
				}
				if (norm){
					ifile>>s;
					nx = std::stof(s);
					ifile>>s;
					ny = std::stof(s);
					ifile>>s;
					nz = std::stof(s);
					p->setNormal(nx,ny,nz);
				}
				if (bx){
					ifile>>s;
					nx = std::stof(s);
					ifile>>s;
					ny = std::stof(s);
					ifile>>s;
					nz = std::stof(s);
					ifile>>s;
					nx = std::stof(s);
					ifile>>s;
					ny = std::stof(s);
					ifile>>s;
					nz = std::stof(s);
				}
				if (map){
					ifile>>s;
					t = std::stof(s);
					ifile>>s;
					r = std::stof(s);
					p->setTexture(t, r);
				}
			}
			float avgx = (maxx+minx)/2,
				  avgy = (maxy+miny)/2,
				  avgz = (maxz+minz)/2;
			float maxval = -9999;
			if (fabs(maxx - minx) > fabs(maxy - miny) && fabs(maxx - minx) > fabs(maxz - minz))
				maxval = maxx - minx;
			else if (fabs(maxy - miny) > fabs(maxz - minz))
				maxval = maxy - miny;
			else
				maxval = maxz - minz;
			
			for ( i=0; i < num_vertex; i++){
				points[i]->setX((points[i]->getX() - avgx)/maxval);
				points[i]->setY((points[i]->getY() - miny)/maxval);
				points[i]->setZ((points[i]->getZ() - avgz)/maxval);
			}
			minx = (minx - avgx)/maxval;
			miny = (miny - avgy)/maxval;
			minz = (minz - avgz)/maxval;
			
			maxx = (maxx - avgx)/maxval;
			maxy = (maxy - avgy)/maxval;
			maxz = (maxz - avgz)/maxval;
			std::cout<<avgx<<std::endl;
			std::cout<<avgy<<std::endl;
			std::cout<<avgz<<std::endl;
			
			std::cout<<minx<<" "<<maxx<<std::endl;
			std::cout<<miny<<" "<<maxy<<std::endl;
			std::cout<<minz<<" "<<maxz<<std::endl;
			std::cout<<"Maxval: "<<maxval<<std::endl;
			for ( i=0; i < num_faces; i++){
				polygon = new Polygon();
				ifile>>s;
				sides = std::stoi(s);
				for ( j=0; j < sides; j++){
					ifile>>s;
					vertex_idx = std::stoi(s);
					polygon->addVertex(points[vertex_idx]);
				}
				polygons.push_back(polygon);
			}
			std::cout<<points.size()<<std::endl;
			std::cout<<polygons.size()<<std::endl;
		}
	}
	ifile.close();
}
// Read object from ply file
void Model::readPlyFileAsIs(char *filename){
	std::cout<<filename<<std::endl;
	std::ifstream ifile;
	ifile.open(filename);
	std::string s;
	long int num_vertex, num_faces; long long i;
	float x, y, z, nx, ny, nz, t, r;
	int co_in = 0, bx = 0;
	int vertex_idx, j, sides = 0;
	Polygon* polygon;
	Point *p;
	while(ifile>>s){
		if (s.compare("vertex") == 0){
			ifile>>s;
			num_vertex = std::stoi(s);
			std::cout<<num_vertex<<std::endl;
		}else if (s.compare("face") == 0){
			ifile>>s;
			num_faces = std::stoi(s);
			std::cout<<num_faces<<std::endl;
		}else if (s.compare("confidence") == 0 || s.compare("intensity") == 0){
			co_in = 1;
		}else if (s.compare("nx") == 0){
			flg_nx = 1;
		}else if (s.compare("t") == 0){
			flg_ts = 1;
		}else if (s.compare("bx") == 0){
			bx = 1;
		}else if (s.compare("end_header") == 0){
			for ( i=0; i < num_vertex; i++){
				ifile>>s;
				x = std::stof(s);
				ifile>>s;
				y = std::stof(s);
				ifile>>s;
				z = std::stof(s);
				p = new Point(x, y, z);
				points.push_back(p);
				if (minx > x)
					minx = x;
				if (miny > y)
					miny = y;
				if (minz > z)
					minz = z;
				if (maxx < x)
					maxx = x;
				if (maxy < y)
					maxy = y;
				if (maxz < z)
					maxz = z;
				if (co_in){
					ifile>>s;
					ifile>>s;
				}
				if (flg_nx){
					ifile>>s;
					nx = std::stof(s);
					ifile>>s;
					ny = std::stof(s);
					ifile>>s;
					nz = std::stof(s);
					p->setNormal(nx,ny,nz);
				}
				if (flg_ts){
					ifile>>s;
					t = std::stof(s);
					ifile>>s;
					r = std::stof(s);
					p->setTexture(t, r);
				}
				if (bx){
					ifile>>s;
					nx = std::stof(s);
					ifile>>s;
					ny = std::stof(s);
					ifile>>s;
					nz = std::stof(s);
					ifile>>s;
					nx = std::stof(s);
					ifile>>s;
					ny = std::stof(s);
					ifile>>s;
					nz = std::stof(s);
				}
			}
			for ( i=0; i < num_faces; i++){
				polygon = new Polygon();
				ifile>>s;
				sides = std::stoi(s);
				for ( j=0; j < sides; j++){
					ifile>>s;
					vertex_idx = std::stoi(s);
					polygon->addVertex(points[vertex_idx]);
				}
				polygons.push_back(polygon);
			}
			std::cout<<points.size()<<std::endl;
			std::cout<<polygons.size()<<std::endl;
		}
	}
	std::cout<<"Done Reading"<<std::endl;
	ifile.close();
}
// compute normals for each polygon of the object
void Model::computeNormals(){
	long int i;
	if (flg_ts)
		return;
	
	std::cout<<"Normals"<<std::endl;
	for(i=0;i < polygons.size();i++)
		polygons[i]->computeNormal();
	std::cout<<"Normalise"<<std::endl;
	for(i=0;i < polygons.size();i++)
		polygons[i]->normalize();
}
// Render bounding box of the object
void Model::drawBoundingBox(){
	glBegin(GL_LINES);
	glColor4f(0.4, 0.1, 0.25, 0);
	glVertex3f(minx, miny, minz);
	glVertex3f(maxx, miny, minz);
	
	glVertex3f(maxx, miny, minz);
	glVertex3f(maxx, maxy, minz);
	
	glVertex3f(maxx, maxy, minz);
	glVertex3f(minx, maxy, minz);
	
	glVertex3f(minx, maxy, minz);
	glVertex3f(minx, miny, minz);
	
	glEnd();
	
	glBegin(GL_LINES);
	maxy -= miny;
	miny = 0;
	glColor4f(1, 0, 0.2, 0);
	glVertex3f(minx, miny, maxz);
	glVertex3f(maxx, miny, maxz);
	
	glVertex3f(maxx, miny, maxz);
	glVertex3f(maxx, maxy, maxz);
	
	glVertex3f(maxx, maxy, maxz);
	glVertex3f(minx, maxy, maxz);
	
	glVertex3f(minx, maxy, maxz);
	glVertex3f(minx, miny, maxz);
	glEnd();
	
	glBegin(GL_LINES);
	glColor4f(1, 1, 0, 0);
	glVertex3f(minx, miny, minz);
	glVertex3f(minx, miny, maxz);
	
	glVertex3f(maxx, miny, minz);
	glVertex3f(maxx, miny, maxz);
	glEnd();
	
	glBegin(GL_LINES);
	glColor4f(1, 1, 0, 0);
	glVertex3f(minx, maxy, minz);
	glVertex3f(minx, maxy, maxz);
	
	glVertex3f(maxx, maxy, minz);
	glVertex3f(maxx, maxy, maxz);
	glEnd();
}
// update next position of the model
void Model::nextPosition(float *transm, float *rotsm, float* scle){
}
// update direction of the model
void Model::setDirection(GLfloat *pos){
	
}

/*
 * Light class inherited from Model- represents a light in the scene
 */
// Light constructor
Light::Light() : Model(){
	std::cout<<"Light Init"<<std::endl;
	GLfloat dif[] = { 1.0, .0, .0, 1.0 };
	GLfloat spec[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat position[] = { 0.5, 5, 0, 1};
	glLightfv(GL_LIGHT7, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT7, GL_SPECULAR, spec);
	glLightf(GL_LIGHT7,GL_SPOT_CUTOFF, 10);
	glLightf(GL_LIGHT7,GL_SPOT_EXPONENT, 100);
	glLightfv (GL_LIGHT7, GL_POSITION, position);
	glEnable(GL_LIGHT7);
}
// Render Light
void Light::drawObject(){
	//~ std::cout<<"Light Action: "<<std::endl;
	//~ glColor3f(1, 0, 1);
	//~ glPushMatrix();
	//~ glTranslatef(  , 0.5, 0);
	//~ glutWireSphere(0.05, 10, 10);
	//~ glPopMatrix();
}
// Set light direction
void Light::setDirection(GLfloat *pos){
	std::cout<<"Light: "<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<" "<<std::endl;
	pos[0] -= 0.5;
	pos[1] -= 5;
	//~ pos[2] -= 3;
	pos[3] = 0;
	GLfloat position[] = { 0.5, 5, 0, 1};
	float no = sqrt(pos[0]*pos[0] + pos[1]*pos[1]+pos[2]*pos[2]);
	pos[0] /= no; pos[1] /= no; pos[2] /= no;
	glLightfv (GL_LIGHT7, GL_POSITION, position);
	glLightfv (GL_LIGHT7, GL_SPOT_DIRECTION, pos);
}
// Set next postion of the light
void Light::nextPosition(float *transm, float *rotsm, float* scle){}

/*
 * Tankhead class inherited from Model- represents a Tank head in the scene
 */
// Tankhead constructor
Tankhead::Tankhead(char *fname): Model(fname){
	time = 0;
}
// Render Tankhead
void Tankhead::drawObject(){
	Model::drawObject();
}
// set next position of the tank head
void Tankhead::nextPosition(float *transm, float *rotsm, float* scle){
	if (time <=10)
		rotsm[1] += 6;
	else if (time <=20)
		rotsm[1] -= 6;
	else
		time = 0;
	time++;
}

/*
 * Tankbase class inherited from Model- represents a Tank base in the scene
 */
// Tankbase constructor
Tankbase::Tankbase(char *fname): Model(fname){
	time = 0;
}
// Render tank base
void Tankbase::drawObject(){
	Model::drawObject();
}
// set next position of the tank head
void Tankbase::nextPosition(float *transm, float *rotsm, float* scle){
	if (time <=40)
		transm[2] += 0.02;
	else if (time <=80)
		transm[2] -= 0.02;
	else
		time = 0;
	time++;
}

/*
 * Ship class inherited from Model- represents a ship in the scene
 */
// Ship constructor
Ship::Ship(char *fname): Model(fname){
	time = 0;
}
// Render ship object
void Ship::drawObject(){
	Model::drawObject();
}
// set next position of the ship object
void Ship::nextPosition(float *transm, float *rotsm, float* scle){
	transm[0] += 0.005;
	time ++;
}

/*
 * Jet class inherited from Model- represents a jet in the scene
 */
// Jet constructor
Jet::Jet(char *fname): Model(fname){
	time = 0;
}
// Render jet object
void Jet::drawObject(){
	std::cout <<"Jet: "<<time<<std::endl;
	Model::drawObject();
}
// set next position of the jet object
void Jet::nextPosition(float *transm, float *rotsm, float* scle){
	if (time <= 30){
		transm[0] -= 0.28;
		transm[1] -= 0.03;
	}else if (time>30 && time <=90){
		transm[0] = 0.0; transm[1] = 0.01; transm[2] = 0.0;
		rotsm[1] = -180; *scle = 0.4;
	}else if (time == 91){
		transm[0] = -3.5; transm[1] = 2.3; transm[2] = -4.8;
		rotsm[1] = 90;
		*scle = 0.8;
	}else if (time <= 120){
		transm[1] -= 0.05; transm[2] += 0.05;
		rotsm[2] += 12;
	}else if (time <= 145){
		transm[1] -= 0.009; transm[0] += 0.05; transm[2] += 0.005;
		rotsm[2] = 0.0;
	}else if (time <=180){
		transm[1] = 1.15; transm[0] = 0.5; transm[2] = 0.0;
		rotsm[1] = 0.0; rotsm[2] = 0.0; *scle = 1.6;
	}
	time++;
}

/*
 * Spaceship class inherited from Model- represents a Spaceship in the scene
 */
// Spaceship constructor
Spaceship::Spaceship(char *fname): Model(fname){
	time = 0;
}
// Render spaceship object
void Spaceship::drawObject(){
	Model::drawObject();
}
// set next position of spaceship object
void Spaceship::nextPosition(float *transm, float *rotsm, float* scle){
	if (time <=30)
		transm[0] -= 0.11;
	else if (time >30 && time <=60){
		rotsm[1] -= 2;
		rotsm[2] -= 2;
		transm[0] -= 0.2;
		transm[2] -= 0.18;
	}else if (time > 60 && time <=90){
		rotsm[1] -= 3;
		rotsm[2] += 2;
		transm[0] += 0.2;
		transm[2] -= 0.15;
	}else if (time > 90 && time < 120){
		transm[2] -=0.05;
		transm[0] += 0.1;
	}
	time++;
}

/*
 * SceneGraph class - represents the scenegraph which defines the scene
 */
// SceneGraph constructor
SceneGraph::SceneGraph(){
	model = new Model();
	trans_matrix[0] = 0;
	trans_matrix[1] = 0;
	trans_matrix[2] = 0;
	rot_matrix[0] = 0;
	rot_matrix[1] = 0;
	rot_matrix[2] = 0;
	scale = 1;
}
// SceneGraph constructor
SceneGraph::SceneGraph(Model *m){
	model = m;
	trans_matrix[0] = 0;
	trans_matrix[1] = 0;
	trans_matrix[2] = 0;
	rot_matrix[0] = 0;
	rot_matrix[1] = 0;
	rot_matrix[2] = 0;
	scale = 1;
}
// SceneGraph constructor
SceneGraph::SceneGraph(char* fname){
	model = new Model(fname);
	trans_matrix[0] = 0;
	trans_matrix[1] = 0;
	trans_matrix[2] = 0;
	rot_matrix[0] = 0;
	rot_matrix[1] = 0;
	rot_matrix[2] = 0;
	scale = 1;
}
// SceneGraph constructor
SceneGraph::SceneGraph(float* trans_mat){
	model = new Model();
	trans_matrix[0] = trans_mat[0];
	trans_matrix[1] = trans_mat[1];
	trans_matrix[2] = trans_mat[2];
	rot_matrix[0] = trans_mat[3];
	rot_matrix[1] = trans_mat[4];
	rot_matrix[2] = trans_mat[5];
	scale = trans_mat[6];
}
// SceneGraph constructor
SceneGraph::SceneGraph(char* fname, float* trans_mat){
	model = new Model(fname);
	trans_matrix[0] = trans_mat[0];
	trans_matrix[1] = trans_mat[1];
	trans_matrix[2] = trans_mat[2];
	rot_matrix[0] = trans_mat[3];
	rot_matrix[1] = trans_mat[4];
	rot_matrix[2] = trans_mat[5];
	scale = trans_mat[6];
}
// SceneGraph constructor
SceneGraph::SceneGraph(char* fname, char* tex, float* trans_mat){
	model = new Model(fname);
	trans_matrix[0] = trans_mat[0];
	trans_matrix[1] = trans_mat[1];
	trans_matrix[2] = trans_mat[2];
	rot_matrix[0] = trans_mat[3];
	rot_matrix[1] = trans_mat[4];
	rot_matrix[2] = trans_mat[5];
	scale = trans_mat[6];
	
}
// SceneGraph constructor
SceneGraph::SceneGraph(Model *m, float* trans_mat){
	model = m;
	trans_matrix[0] = trans_mat[0];
	trans_matrix[1] = trans_mat[1];
	trans_matrix[2] = trans_mat[2];
	rot_matrix[0] = trans_mat[3];
	rot_matrix[1] = trans_mat[4];
	rot_matrix[2] = trans_mat[5];
	scale = trans_mat[6];
}
// set model to this scenegraph node
void SceneGraph::setModel(Model *m){
	model = m;
}
// get model of this scenegraph node
Model* SceneGraph::getModel(){
	return model;
}
// set model transitions
void SceneGraph::setTransitions(){
	glTranslatef(trans_matrix[0], trans_matrix[1], trans_matrix[2]);
	glRotatef(rot_matrix[0], 1, 0, 0);
	glRotatef(rot_matrix[1], 0, 1, 0);
	glRotatef(rot_matrix[2], 0, 0, 1);
	glScalef(scale, scale, scale);
}
// add texture to the model in this scenegraph node
void SceneGraph::addTexture(char *tex){
	model->readTexture(tex);
}
// draw Model corresponding to this scenegraph node
void SceneGraph::drawModel(){
	if (dynamic_cast<Light*>(model) != NULL){
		GLfloat pos[4];
		if (scene_models.size() >= 1){
			scene_models[0]->getPosition(&pos[0]);
			model->setDirection(&pos[0]);
		}
	}
	if (box)
		model->drawBoundingBox();
	if (texture){
		glEnable(GL_TEXTURE_2D);
		model->setTexture();
		model->drawObject();
		glDisable(GL_TEXTURE_2D);
	}else{
		model->drawObject();
	}
}
// Draw the complete scenegraph
void SceneGraph::drawGraph(){
	glPushMatrix();
	setTransitions();
	drawModel();
	int i = 0;
	for (i=0; i < scene_models.size(); i++)
		scene_models[i]->drawGraph();
	glPopMatrix();
	model->nextPosition(trans_matrix, rot_matrix, &scale);
	return;
}
// Remove scene graph node
SceneGraph* SceneGraph::popSceneNode(){
	SceneGraph *sg = scene_models[0];
	scene_models.erase(scene_models.begin()+0);
	return sg;
}
// compute model normal
void SceneGraph::computeModelNormal(){
	model->computeNormals();
	int i = 0;
	for (i=0; i < scene_models.size(); i++)
		scene_models[i]->computeModelNormal();
	return;
}
// Add child scenegraph node
void SceneGraph::appendModelNode(SceneGraph* sm){
	scene_models.push_back(sm);
}
void SceneGraph::texSphericalMap(){
	model->textureMapSphere();
}
void SceneGraph::texCylindricalMap(){
	model->textureMapCylinder();
}
void SceneGraph::getPosition(GLfloat *pos){
	pos[0] = trans_matrix[0];
	pos[1] = trans_matrix[1];
	pos[2] = trans_matrix[2];
}
// return child scenegraph node at i
SceneGraph* SceneGraph::getNode(int i){
	return scene_models[i];
}
// update node transitions
void SceneGraph::updateTransitions(float *pos){
	trans_matrix[0] += pos[0];
	trans_matrix[1] += pos[1];
	trans_matrix[2] += pos[2];
	rot_matrix[0] += pos[3];
	rot_matrix[1] += pos[4];
	rot_matrix[2] += pos[5];
}

/*
 * Scene class - represents the complete scene
 */
// Scene constructor
Scene::Scene(){
	// Creating Ambient Light
	changecam = 0;
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	lights.push_back(GL_LIGHT0);
	glLightfv(lights[0], GL_AMBIENT, light_ambient);
}
// initialize the scene
void Scene::init(char **argv){
	char text1[] = "../bin/Tank_Body_Desert.png";
	char text2[] = "../bin/A_Kuznetsov.png";
	char text3[] = "../bin/lamp1.jpg";
	float trans[7] = {0,2,5,0,90,0,2};
	Spaceship *ss = new Spaceship(argv[3]); 
	SceneGraph *sg = new SceneGraph(ss, &trans[0]);
	sg->texSphericalMap();
	trans[0] = 5.; trans[1] = 3.0; trans[2] = 5;
	trans[4] = -90; trans[6] = 0.8;
	Jet *j = new Jet(argv[4]);
	
	SceneGraph *sg1 = new SceneGraph(j, &trans[0]);
	sg1->addTexture(text3);
	sg1->texCylindricalMap();
	Light *lig1 = new Light();
	SceneGraph *sg2 = new SceneGraph(lig1);
	sg2->appendModelNode(sg);
	
	char fship[] = "../bin/ship.ply";
	trans[0] = -0.25;
	trans[1] = 1.;
	trans[2] = 1.5;
	trans[4] = -90;
	trans[6] = 0.5;
	
	Tankbase *tankbase = new Tankbase(argv[1]);
	SceneGraph *sg3 = new SceneGraph(tankbase, &trans[0]);
	sg3->addTexture(text1);
	sg3->texSphericalMap();
	
	trans[0] = 0.;
	trans[1] = 0.7;
	trans[2] = 0.;
	trans[4] = 60;
	trans[6] = 1;
	
	Tankhead *tankhead = new Tankhead(argv[2]);
	SceneGraph *sg4 = new SceneGraph(tankhead, &trans[0]);
	sg4->addTexture(text1);
	sg4->texCylindricalMap();
	
	trans[0] = -3;
	trans[1] = 0.0;
	trans[2] = -3.0;
	trans[4] = 90;
	trans[6] = 0.5;
	
	Ship *ship = new Ship(fship);
	SceneGraph *sg5 = new SceneGraph(ship, &trans[0]);
	sg5->addTexture(text2);
	sg5->appendModelNode(sg3);
	sg3->appendModelNode(sg4);
	addModel(sg5);
	addModel(sg2);
	addModel(sg1);
	time = 0;
}
// set scenegraph 
void Scene::setSceneGraph(std::vector<SceneGraph*> sg){
	scene_graph = sg;
}
// Render the scene
void Scene::drawScene(){
	updateScene();
	int i;
	for (i=0; i < scene_graph.size(); i++)
		scene_graph[i]->drawGraph();
	time++;
	return;
}
// add scenegraph 
void Scene::addModel(SceneGraph* sg){
	scene_graph.push_back(sg);
}
void Scene::computeModelNormals(){
	int i;
	for (i=0; i < scene_graph.size(); i++)
		scene_graph[i]->computeModelNormal();
	return;
}
// Add light into the scene
void Scene::addLight(float *amb, float *dif, float *spec, float cutoff, float exp){
	int light = lights.back()+1;
	lights.push_back(light);
	
	// Set Light Properties
	glLightfv(light, GL_AMBIENT, amb);
	glLightfv(light, GL_DIFFUSE, dif);
	glLightfv(light, GL_SPECULAR, spec);
	glLightf(light,GL_SPOT_CUTOFF, cutoff);
	glLightf(light,GL_SPOT_EXPONENT, exp);
}
void Scene::setLightPosition( int idx, float *position){
	glLightfv (lights[idx], GL_POSITION, position);
}
void Scene::setSpotDirection( int idx, float *spotlight){
	glLightfv (lights[idx], GL_SPOT_DIRECTION, spotlight);
}
void Scene::updateLightState(int idx, int flag){
	if (flag)
		glEnable(lights[idx]);
	else
		glDisable(lights[idx]);
}
// Render x, y and z axis
void Scene::drawAxis(){
	glBegin(GL_LINES);
	float axis = 4;
	//drawing x -axis
	glColor4f(0, 0, 1, 0);
	glVertex3f( axis, 0, 0);
	glVertex3f(-axis, 0, 0);
	
	//drawing y -axis
	glColor4f(0, 1, 0, 0);
	glVertex3f(0, axis, 0);
	glVertex3f(0, -axis,0);
	
	//drawing z -axis
	glColor4f(1, 0, 0, 0);
	glVertex3f(0, 0, axis);
	glVertex3f(0, 0, -axis);
	glEnd();
}
// Render the floor
void Scene::drawFloor(float size, int tex){
	int i, j, div = 20;
	float spc = (float)2.0*size/div;
	float txtspc = 1.0/div;
	glEnable (GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, tex);

	glBegin(GL_QUADS);
	glColor4f(1,1,1,0);
	for (i=0; i< div; i++){
		for (j=0; j < div; j++){
			glTexCoord2f(i*txtspc, j*txtspc); glVertex3f( i*spc-size, 0, j*spc-size);
			glTexCoord2f(i*txtspc, (j+1)*txtspc); glVertex3f( i*spc-size, 0, (j+1)*spc-size);
			glTexCoord2f((i+1)*txtspc, (j+1)*txtspc); glVertex3f(	(i+1)*spc-size, 0, (j+1)*spc-size);
			glTexCoord2f((i+1)*txtspc, j*txtspc); glVertex3f( (i+1)*spc-size, 0, j*spc-size);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
// Render background
void Scene::drawBackground(float r, int tex){
	int i, j, lats = 100, longs = 100;
	float pi = 3.142;
	float txtspc = 1.0/(float)lats;
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, tex);
	glColor4f(1,1,1,0);
	for(i = 0; i <= lats; i++) {
		double lat0 = pi * (-0.5 + (double) (i - 1) / lats);
		double z0  = r*sin(lat0);
		double zr0 =  r*cos(lat0);

		double lat1 = pi * (-0.5 + (double) i / lats);
		double z1 = r*sin(lat1);
		double zr1 = r*cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= longs; j++) {
			double lng = 2 * pi * (double) (j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glTexCoord2f(i*txtspc, j*txtspc);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glTexCoord2f((i+1)*txtspc, (j+1)*txtspc);
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}
void Scene::readObjFile(char* fname){
}
void Scene::updateMovement(float *t){
	scene_graph[0]->updateTransitions(t);
}
// Set camera position
void Scene::setCamera(float *trans, float *rotmat, int changecam){
	
	eye[0] = 7+trans[0];
	eye[1] = 3+trans[1];
	eye[2] = 1+trans[2]-trans[6];
	eye[3] = 1;
	
	if (changecam){
		GLfloat pos[4];
		(scene_graph[1]->getNode(0))->getPosition(&pos[0]);
		eye[0] = pos[0] + trans[0];
		eye[1] = pos[1] + trans[1];
		eye[2] = pos[2] + trans[2]-trans[6];
		eye[3] = 1;
		multMatrix(&eye[0], rotmat);
		gluLookAt
		(
			pos[0], pos[1], pos[2],
			0+eye[0], 0+eye[1], 0+eye[2],
			0, 1, 0
		);
	}else{
		multMatrix(&eye[0], rotmat);
		gluLookAt
		(
			eye[0], eye[1], eye[2],
			0+trans[0], 0+trans[1], 0+trans[2],
			0, 1, 0
		);
	}
	
}
// Compute matrix multiplication of e and rot 
void Scene::multMatrix(float *e, float *rot ){
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
// Update scene properties
void Scene::update(int b, int t){
	if (b)
		box = (box)?0: 1;
	if (t)
		texture = (texture)?0: 1;
}
// Update scene object positions
void Scene::updateScene(){
	if (time == 30){
		// Attaching the object jet to spaceship
		SceneGraph *sg = scene_graph[2];
		(scene_graph[1]->getNode(0))->appendModelNode(sg);
		scene_graph.erase(scene_graph.begin()+2);
	}else if (time == 92){
		// Detaching the object jet from spaceship
		SceneGraph *sg = (scene_graph[1]->getNode(0))->popSceneNode();
		scene_graph.push_back(sg);
	}else if (time == 147){
		// Attaching the jet to the ship
		SceneGraph *sg = scene_graph[2];
		scene_graph[0]->appendModelNode(sg);
		scene_graph.erase(scene_graph.begin()+2);
	}
}
