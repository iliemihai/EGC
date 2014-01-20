#include <iostream>
#include <stdlib.h>
#include "incarca_imaginea.h"
#include "md2.h"
#include "cam.h"
using namespace std;

const float FLOOR_TEXTURE_SIZE = 600; //The size of each floor "tile"
float alfa;
float unghi_initial=0.0f,unghi_final=0.0f;
float _anglex = 30.0f;
float x,y;
float _angley=0.0f;
float _angle;
float x_inamic=0.0f;
float y_inamic=0.0f;
int soarec;
MD2Model* _model;
MD2Model* _model1;
MD2Model* _model2;
Camera camera;
int _textureId;
//The forward position of the guy relative to an arbitrary floor "tile"
float _guyPos_inainte = 0;
float _guyPos_inapoi = 0;
float _guyPos_dreapta = 0;
float _guyPos_stanga = 0;
int coordonata;
void cleanup() {
	delete _model;
	delete _model2;
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			{
				cleanup();
				exit(0);
				break;
			}
 
 
		case 'a':
			camera.rotateFPS_OY(-0.1);
			break;
 
		case 'r':
			camera.translate_Right(0.5);
			break;
		case 'f':
			camera.translate_Right(-0.5);
			break;
		case 't':
			camera.translate_Up(0.5);
			break;
		case 'g':
			camera.translate_Up(-0.5);
			break;
		case 'd':
			camera.rotateFPS_OY(0.1);
			break;
		case 'z':
			camera.rotateFPS_OX(0.1);
			break;
		case 'x':
			camera.rotateFPS_OX(-0.1);
			break;
		case 'v':
			camera.rotateFPS_OZ(0.1);
			break;
		case 'b':
			camera.rotateFPS_OZ(-0.1);
			break;
 
		case 'n':
			camera.rotateTPS_OX(-0.1, 10);
			break;
		case 'm':
			camera.rotateTPS_OX(0.1, 10);
			break;

		case 'k':
			camera.rotateTPS_OZ(-0.1, 10);
			break;
		case 'l':
			camera.rotateTPS_OZ(0.1, 10);
			break;
	}
}

void pressKey(int key, int xx, int yy) {
	switch (key) {
		case 27: //Escape key
			{
				cleanup();
				exit(0);
				break;
			}
		//punctele d miscare
		case  GLUT_KEY_UP:
			{
				_model->setAnimation("run"); 
				coordonata=1;
				camera.translate_Forward(0.018);
				break;
			}
		case  GLUT_KEY_LEFT:
			{
				_model->setAnimation("jump"); 
				coordonata=2;
			camera.rotateTPS_OY(0.1,1);
				 
				 
				break;
			}
		case  GLUT_KEY_RIGHT:
			{
				_model->setAnimation("jump"); 
				coordonata=3;
				camera.rotateTPS_OY(-0.1,1);
				 
				 
				break;
			}
		case  GLUT_KEY_DOWN:
			{
				_model->setAnimation("run"); 
				coordonata=4;
				camera.translate_Forward(-0.018);
				 
				break;
			}
	}
}


void mouseMove(int x, int y) { 	
	if(soarec==1)
	    camera.rotateTPS_OY(0.05,1);
    if(soarec==2)
	    camera.rotateTPS_OY(-0.05,1);
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) 
	{	 
		soarec=2;
	}
	if(button == GLUT_RIGHT_BUTTON)
	{ 
		soarec=1;
	}
	if(button == GLUT_MIDDLE_BUTTON )
	{
		alfa-=45;
	}
}


void releaseKey(int key, int x, int y)
{
	switch (key) {
	
		//punctele d miscare
		case  GLUT_KEY_UP:
			{
				coordonata=1;
				
				_model->setAnimation("jump"); 
				break;
			}
		case  GLUT_KEY_RIGHT:
			{
				_model->setAnimation("jump"); 
				coordonata=3;
				break;
			}
		case  GLUT_KEY_LEFT:
			{
				_model->setAnimation("jump"); 
				coordonata=2;
	
				break;
			}
		case  GLUT_KEY_DOWN:
			{
				_model->setAnimation("jump"); 
				coordonata=4;
				
				break;
			}
	}
}

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image *image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,image->width, image->height,0,GL_RGB,GL_UNSIGNED_BYTE,image->pixels);
	return textureId;
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	//functii pt iluminare
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	
	//Load the model
	_model = MD2Model::load("tallguy.md2");
	_model1= MD2Model::load("tallguy1.md2");
    _model2= MD2Model::load("tallguy2.md2");
	//Load the floor texture
	Image* image3 = loadBMP("stadion2.bmp");
	_textureId = loadTexture(image3);
	delete image3;

	//incarcam textura pentru inamic 1
	Image* image1 = loadBMP("tallguy1.bmp");
	GLuint textureId1 = loadTexture(image1);
	delete image1;
	
	_model1->textureId = textureId1;

	//incarcam textura pentru inamic 2
	Image* image2 = loadBMP("tallguy2.bmp");
	GLuint textureId2 = loadTexture(image2);
	delete image2;
	
	_model2->textureId = textureId2;


	//incarcam textura pentru jucator
	Image* image = loadBMP("tallguy.bmp");
	GLuint textureId = loadTexture(image);
	delete image;
	
	_model->textureId = textureId;
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
 
	//cea mai nenorocita ecuatie,am facut de mana rotirea in jurul unui punct :D
		//setam camera jucator
	camera.render();
	
	 
	//cod pt lumina,
	GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	GLfloat lightColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat lightPos[] = {-0.2f, 0.3f, -1, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	//aici voi scrie cod pt inamici
	
	//ecuatiile de miscare
	//miscam in sus
	if(x_inamic<700)
		x_inamic+=1;
		
	 //la stanga
	  if(y_inamic<650)
	  if(x_inamic>600)
		 y_inamic+=1; 
	  
	  //in jos
	  if(x_inamic>-200)
	  if(y_inamic==650)
		 {x_inamic-=2; 
	   }
	  
	  //la dreapta
	  
	    if(y_inamic>650)
	  if(x_inamic<600)
		 y_inamic-=2;

	//transformari de miscare pt inamic 1
	glPushMatrix();
		glTranslatef(5.0f, 4.5f, 70.0f);
		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glScalef(0.5f, 0.5f, 0.5f);
		glTranslatef(0,y_inamic,x_inamic);
		if(x_inamic>600)
			glRotatef(-90,1,0,0);
		if(y_inamic>648)
			glRotatef(-180,1,0,0);
		_model1->draw();
		_model1->setAnimation("run"); 
		_model1->advance(0.1f);
	glPopMatrix();


	//transformari de miscare pt inamic 2
	glPushMatrix();
		glTranslatef(5.0f, 4.5f, 80.0f);
		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glScalef(0.5f, 0.5f, 0.5f);
		glTranslatef(0,y_inamic,x_inamic);
		if(x_inamic>600)
			glRotatef(-90,1,0,0);
		if(y_inamic>648)
			glRotatef(-180,1,0,0);
		_model2->draw();
		_model2->setAnimation("run"); 
		_model2->advance(0.1f);
	glPopMatrix();

	Vector3D pos(camera.position +camera.forward  );//mai trebe adaugat un vector pt a vedea mai bine
 cout<<camera.position.x<<" "<<camera.position.y<<" "<<camera.position.z<<" "<<endl;
	//animatie
	if (coordonata==1) {
		glPushMatrix();
		glTranslatef(15, 4.5f, 85.0f);
			glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glScalef(0.5f, 0.5f, 0.5f);
		glTranslatef(0,-pos.z*2+170,0);
		glTranslatef(0,0, pos.x *2);
		glRotatef(alfa,1,0,0);
		  _model->draw();
		glPopMatrix();
		_model->advance(0.1f);
		
	}else if(coordonata==2) {
		glPushMatrix();
		glTranslatef(15, 4.5f, 85.0f);
		glRotatef( 180.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.5f, 0.5f, 0.5f);
		glTranslatef(0,0,0);
		glTranslatef(0,0,-pos.z*2+170);
		glTranslatef(0,-pos.x *2,0);
		  _model->draw();
		glPopMatrix();
		_model->advance(0.1f);
		
	}else if(coordonata==3) {
		glPushMatrix();
		glTranslatef(15, 4.5f, 85.0f);
		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.5f, 0.5f, 0.5f);
		glTranslatef(0,0,0);
		glTranslatef(0,0,pos.z*2-170);
		glTranslatef(0,pos.x *2,0);
		 _model->draw();
		glPopMatrix();
		_model->advance(0.1f);
		
	}else if(coordonata==4) {
		glPushMatrix(); 
		glTranslatef(15, 4.5f, 85.0f);
		glRotatef(+90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef( -90.0f, 0.0f, 1.0f, 0.0f);
		glScalef(0.5f, 0.5f, 0.5f);
		glTranslatef(0,pos.z*2-170,0);
		glTranslatef(0,0 ,-pos.x *2);
		glRotatef(alfa,1,0,0);
		  _model->draw();
		glPopMatrix();
		_model->advance(0.1f);
	
	}
	

	//Desenam podeaua
	 
	glTranslatef(0.0f, -5.4f, 0.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	
	if(coordonata==1)
	{
		//inainte
		glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE, 0 / FLOOR_TEXTURE_SIZE+ 0 / FLOOR_TEXTURE_SIZE);
			glVertex3f(-1000.0f, 0.0f, -1000.0f);
			 
			glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE,(2000 + 0) / FLOOR_TEXTURE_SIZE);
			glVertex3f(-1000.0f, 0.0f, 1000.0f);
			 
			glTexCoord2f((0), (2000 + 0) / FLOOR_TEXTURE_SIZE);
			glVertex3f(1000.0f, 0.0f, 1000.0f);
			 
			glTexCoord2f(0.0f,(0)  / FLOOR_TEXTURE_SIZE+ (0) / FLOOR_TEXTURE_SIZE);
			glVertex3f(1000.0f, 0.0f, -1000.0f);

	}else if(coordonata==2)//d
	{
		glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE, 0 / FLOOR_TEXTURE_SIZE+ 0 / FLOOR_TEXTURE_SIZE);
			glVertex3f(-1000.0f, 0.0f, -1000.0f);
			 
			glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE,(2000 + 0) / FLOOR_TEXTURE_SIZE);
			glVertex3f(-1000.0f, 0.0f, 1000.0f);
			 
			glTexCoord2f((0), (2000 + 0) / FLOOR_TEXTURE_SIZE);
			glVertex3f(1000.0f, 0.0f, 1000.0f);
			 
			glTexCoord2f(0.0f,(0)  / FLOOR_TEXTURE_SIZE+ (0) / FLOOR_TEXTURE_SIZE);
			glVertex3f(1000.0f, 0.0f, -1000.0f);

	}else if(coordonata==3)//a
	{
		glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE, 0 / FLOOR_TEXTURE_SIZE+ 0 / FLOOR_TEXTURE_SIZE);
			glVertex3f(-1000.0f, 0.0f, -1000.0f);
			 
			glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE,(2000 + 0) / FLOOR_TEXTURE_SIZE);
			glVertex3f(-1000.0f, 0.0f, 1000.0f);
			 
			glTexCoord2f((0), (2000 + 0) / FLOOR_TEXTURE_SIZE);
			glVertex3f(1000.0f, 0.0f, 1000.0f);
			 
			glTexCoord2f(0.0f,(0)  / FLOOR_TEXTURE_SIZE+ (0) / FLOOR_TEXTURE_SIZE);
			glVertex3f(1000.0f, 0.0f, -1000.0f);

	}else if(coordonata==4)
	{
		 
		glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE, 0 / FLOOR_TEXTURE_SIZE+ 0 / FLOOR_TEXTURE_SIZE);
			glVertex3f(-1000.0f, 0.0f, -1000.0f);
			 
			glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE,(2000 + 0) / FLOOR_TEXTURE_SIZE);
			glVertex3f(-1000.0f, 0.0f, 1000.0f);
			 
			glTexCoord2f((0), (2000 + 0) / FLOOR_TEXTURE_SIZE);
			glVertex3f(1000.0f, 0.0f, 1000.0f);
			 
			glTexCoord2f(0.0f,(0)  / FLOOR_TEXTURE_SIZE+ (0) / FLOOR_TEXTURE_SIZE);
			glVertex3f(1000.0f, 0.0f, -1000.0f);
		 
			 
		 
	}
	glEnd();
	glutSwapBuffers();
	 
}

//functia de reafisare
void update(int value) {

	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1366, 768);
	
	glutCreateWindow("Tema 3 PA");
	initRendering();
	camera.init();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0);
	
	glutMainLoop();
	return 0;
}
