#include <iostream>
#include <stdlib.h>
#include "incarca_imaginea.h"
#include "md2.h"

using namespace std;

const float FLOOR_TEXTURE_SIZE = 600; //The size of each floor "tile"
float alfa=0.0f;
float _anglex = 30.0f;
float _angley=0.0f;
MD2Model* _model;
int _textureId;
//The forward position of the guy relative to an arbitrary floor "tile"
float _guyPos_inainte = 0;
float _guyPos_inapoi = 0;
float _guyPos_dreapta = 0;
float _guyPos_stanga = 0;
int coordonata;
void cleanup() {
	delete _model;
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			{
				cleanup();
				exit(0);
				break;
			}
		case 'g':
			{
				_anglex += 0.7f; 
				alfa+=0.1;
				break;
			}
		case 'j':
			{
				_anglex -= 0.7f; 
				alfa-=0.1;
				break;
			}
		case 'y':
			{
				_angley += 0.7f; 
				break;
			}
		case 'h':
			{
				_angley -= 0.7f; 
				break;
			}
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
				_guyPos_inainte += 0.5f;
				break;
			}
		case  GLUT_KEY_RIGHT:
			{
				_model->setAnimation("run"); 
				coordonata=2;
				_guyPos_dreapta += 0.009f;
				 
				break;
			}
		case  GLUT_KEY_LEFT:
			{
				_model->setAnimation("run"); 
				coordonata=3;
				_guyPos_stanga += 0.009f;
				 
				break;
			}
		case  GLUT_KEY_DOWN:
			{
				_model->setAnimation("run"); 
				coordonata=4;
				_guyPos_inapoi += 0.5f;
				break;
			}
	}
}

void releaseKey(int key, int x, int y)
{
	switch (key) {
	
		//punctele d miscare
		case  GLUT_KEY_UP:
			{
				coordonata=1;
				_guyPos_inainte += 0.5f;
				_model->setAnimation("jump"); 
				break;
			}
		case  GLUT_KEY_RIGHT:
			{
				_model->setAnimation("jump"); 
				coordonata=2;
				_guyPos_dreapta += 0.009f;
				
				break;
			}
		case  GLUT_KEY_LEFT:
			{
				_model->setAnimation("jump"); 
				coordonata=3;
				_guyPos_stanga += 0.009f;
				 
				break;
			}
		case  GLUT_KEY_DOWN:
			{
				_model->setAnimation("jump"); 
				coordonata=4;
				_guyPos_inapoi += 0.5f;
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

	//Load the floor texture
	Image* image = loadBMP("stadion2.bmp");
	_textureId = loadTexture(image);
	delete image;
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
	
	glTranslatef(+10,0,0);
 
	//cea mai nenorocita ecuatie,am facut de mana rotirea in jurul unui punct :D
		//setam camera inainte/inaoi
		gluLookAt(30*cos(alfa) ,5.0f,-30*sin(alfa) ,
			30*(-cos(alfa)-sin(alfa)) ,5.0f, 30*(sin(alfa)-cos(alfa) )  ,
			   0.0f,1.0f,0.0f);
	 
	//cod pt lumina,magie
	GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	GLfloat lightColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat lightPos[] = {-0.2f, 0.3f, -1, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	
	//animatie
	if (coordonata==1) {
		glPushMatrix();
		glTranslatef(0.0f, 4.5f, 0.0f);
		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.5f, 0.5f, 0.5f);
		_model->draw();
		glPopMatrix();
		_model->advance(0.06f);
		
	}else if(coordonata==2) {
		glPushMatrix();
		glTranslatef(0.0f, 4.5f, 0.0f);
		glRotatef(+90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef( -90.0f, 0.0f, 1.0f, 0.0f);
		glScalef(0.5f, 0.5f, 0.5f);
		_model->draw();
		glPopMatrix();
		_model->advance(0.06f);
		
	}else if(coordonata==3) {
		glPushMatrix();
		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); 
		glTranslatef(-4.5f, 0.0f, 0.0f);
		glScalef(0.5f, 0.5f, 0.5f);
		_model->draw();
		glPopMatrix();
		_model->advance(0.06f);
		
	}else if(coordonata==4) {
		glPushMatrix();
		glTranslatef(0.0f, 4.5f, 0.0f);
		glRotatef( 180.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.5f, 0.5f, 0.5f);
		_model->draw();
		glPopMatrix();
		_model->advance(0.06f);
		
	}
	
	//Draw the floor
	glTranslatef(0.0f, -5.4f, 0.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 
	
	glBegin(GL_QUADS);
	
	if(coordonata==1)
	{
		//inainte
		glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE,(+_guyPos_stanga/6 -_guyPos_dreapta/6)  / FLOOR_TEXTURE_SIZE+ (+_guyPos_inainte-_guyPos_inapoi) / FLOOR_TEXTURE_SIZE);
			glVertex3f(-1000.0f, 0.0f, -1000.0f);
			 
			glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE,(2000 + _guyPos_inainte-_guyPos_inapoi) / FLOOR_TEXTURE_SIZE);
			glVertex3f(-1000.0f, 0.0f, 1000.0f);
			 
			glTexCoord2f((-_guyPos_stanga/6 +_guyPos_dreapta/6), (2000 + _guyPos_inainte-_guyPos_inapoi) / FLOOR_TEXTURE_SIZE);
			glVertex3f(1000.0f, 0.0f, 1000.0f);
			 
			glTexCoord2f(0.0f,(+ _guyPos_stanga/6 -_guyPos_dreapta/6)  / FLOOR_TEXTURE_SIZE+ (+_guyPos_inainte-_guyPos_inapoi) / FLOOR_TEXTURE_SIZE);
			glVertex3f(1000.0f, 0.0f, -1000.0f);

		 

	}else if(coordonata==2)//d
	{
		glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE,(+_guyPos_stanga/6 -_guyPos_dreapta/6)  / FLOOR_TEXTURE_SIZE+ (+_guyPos_inainte-_guyPos_inapoi) / FLOOR_TEXTURE_SIZE);
			glVertex3f(-1000.0f, 0.0f, -1000.0f);
			 
			glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE,(2000 + _guyPos_inainte-_guyPos_inapoi) / FLOOR_TEXTURE_SIZE);
			glVertex3f(-1000.0f, 0.0f, 1000.0f);
			 
			glTexCoord2f((-_guyPos_stanga/6 +_guyPos_dreapta/6), (2000 + _guyPos_inainte-_guyPos_inapoi) / FLOOR_TEXTURE_SIZE);
			glVertex3f(1000.0f, 0.0f, 1000.0f);
			 
			glTexCoord2f(0.0f,(+ _guyPos_stanga/6 -_guyPos_dreapta/6)  / FLOOR_TEXTURE_SIZE+ (+_guyPos_inainte-_guyPos_inapoi) / FLOOR_TEXTURE_SIZE);
			glVertex3f(1000.0f, 0.0f, -1000.0f);

	}else if(coordonata==3)//a
	{
		glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE,(+_guyPos_stanga/6 -_guyPos_dreapta/6)  / FLOOR_TEXTURE_SIZE+ (+_guyPos_inainte-_guyPos_inapoi) / FLOOR_TEXTURE_SIZE);
			glVertex3f(-1000.0f, 0.0f, -1000.0f);
			 
			glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE,(2000 + _guyPos_inainte-_guyPos_inapoi) / FLOOR_TEXTURE_SIZE);
			glVertex3f(-1000.0f, 0.0f, 1000.0f);
			 
			glTexCoord2f((-_guyPos_stanga/6 +_guyPos_dreapta/6), (2000 + _guyPos_inainte-_guyPos_inapoi) / FLOOR_TEXTURE_SIZE);
			glVertex3f(1000.0f, 0.0f, 1000.0f);
			 
			glTexCoord2f(0.0f,(+ _guyPos_stanga/6 -_guyPos_dreapta/6)  / FLOOR_TEXTURE_SIZE+ (+_guyPos_inainte-_guyPos_inapoi) / FLOOR_TEXTURE_SIZE);
			glVertex3f(1000.0f, 0.0f, -1000.0f);

	}else if(coordonata==4)
	{
		 
		glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE,(+_guyPos_stanga/6 -_guyPos_dreapta/6)  / FLOOR_TEXTURE_SIZE+ (+_guyPos_inainte-_guyPos_inapoi) / FLOOR_TEXTURE_SIZE);
			glVertex3f(-1000.0f, 0.0f, -1000.0f);
			 
			glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE,(2000 + _guyPos_inainte-_guyPos_inapoi) / FLOOR_TEXTURE_SIZE);
			glVertex3f(-1000.0f, 0.0f, 1000.0f);
			 
			glTexCoord2f((-_guyPos_stanga/6 +_guyPos_dreapta/6), (2000 + _guyPos_inainte-_guyPos_inapoi) / FLOOR_TEXTURE_SIZE);
			glVertex3f(1000.0f, 0.0f, 1000.0f);
			 
			glTexCoord2f(0.0f,(+ _guyPos_stanga/6 -_guyPos_dreapta/6)  / FLOOR_TEXTURE_SIZE+ (+_guyPos_inainte-_guyPos_inapoi) / FLOOR_TEXTURE_SIZE);
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
	
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0);
	
	glutMainLoop();
	return 0;
}
