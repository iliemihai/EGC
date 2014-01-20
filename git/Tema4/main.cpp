//-------------------------------------------------------------------------------------------------
// Descriere: fisier main
//
// Autor: student
// Data: today
//-------------------------------------------------------------------------------------------------

//incarcator de meshe
#include "lab_mesh_loader.hpp"
//geometrie: drawSolidochi_drept, drawWireTeapot...
#include "lab_geometry.hpp"
//incarcator de shadere
#include "lab_shader_loader.hpp"
//interfata cu glut, ne ofera fereastra, input, context opengl
#include "lab_glut.hpp"
//texturi
#include "lab_texture_loader.hpp"
//time
#include <ctime>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <vector>
#include <windows.h>
#include <stdio.h>

using namespace std;
#define PI 3.1415926535f 

std::vector<lab::VertexFormat> face_vertices;//vertecsi pt fata
std::vector<unsigned int> face_normala_indices;//indecsi pt fata
std::vector<unsigned int> face_incruntata_indices;//indecsi pt fata
std::vector<unsigned int> face_vesela_indices;//indecsi pt fata
std::vector<lab::VertexFormat> ochi_stang_vertices;//vertecsi pt ochi_stang
std::vector<unsigned int> ochi_stang_indices;//indecsi pt ochi_stang
std::vector<lab::VertexFormat> ochi_drept_vertices;//vertecsi pt ochi_drept
std::vector<unsigned int> ochi_drept_indices;//indecsi pt ochi_drept

float angry=0.0;
float happy=0.0;
float stare;


class Laborator : public lab::glut::WindowListener{

//variabile
private:
	glm::mat4 model_matrix_ochi_stang, model_matrix_fata, model_matrix_ochi_drept;				//matrici modelare pentru cele 3 obiecte
	glm::mat4 model_matrix_dinti_sus, model_matrix_dinti_jos, model_matrix_limba,model_matrix_beregata;
	glm::mat4 view_matrix, projection_matrix;											//matrici 4x4 pt modelare vizualizare proiectie
	unsigned int gl_program_shader1;//pentru animatiile fetei	
	unsigned int gl_program_shader3;//pentru animatiile fetei	//id-ul de opengl al obiectului de tip program shader
	unsigned int gl_program_shader2;//pentru ochi dinti si alte obiecte ce stau fixe
	unsigned int gl_program_shader4;
	unsigned int screen_width, screen_height;

	//meshe
	unsigned int fata_vesela_vbo, fata_vesela_ibo, fata_vesela_vao, fata_vesela_num_indices;
	unsigned int fata_incruntata_vbo, fata_incruntata_ibo, fata_incruntata_vao, fata_incruntata_num_indices;
	unsigned int fata_vbo, fata_ibo, fata_vao, fata_num_indices;					
	unsigned int ochi_stang_vbo, ochi_stang_ibo, ochi_stang_vao, ochi_stang_num_indices;				//containere opengl pentru vertecsi, indecsi si stare
	unsigned int ochi_drept_vbo, ochi_drept_ibo, ochi_drept_vao, ochi_drept_num_indices;		
	unsigned int dinti_sus_vbo,  dinti_sus_ibo,  dinti_sus_vao,  dinti_sus_num_indices;
	unsigned int dinti_jos_vbo,  dinti_jos_ibo,  dinti_jos_vao,  dinti_jos_num_indices;
	unsigned int limba_vbo, limba_ibo, limba_vao, limba_num_indices;
	unsigned int beregata_vbo, beregata_ibo, beregata_vao, beregata_num_indices;

	unsigned int vbo_particles;
		unsigned int ibo_particles;
		unsigned int vao_particles;
	vector<glm::vec3> vertices_particle;
		vector<float> indices_particle;
	unsigned int particula_texture;
	//texturi
	unsigned int fata_vesela_texture,fata_incruntata_texture,fata_texture, ochi_stang_texture_color, ochi_stang_texture_alpha, ochi_drept_texture_color,ochi_drept_texture_alpha;
	unsigned int dinti_sus_texture_color, dinti_sus_texture_alpha, dinti_jos_texture_color,dinti_jos_texture_alpha;
	unsigned int limba_texture_color, limba_texture_alpha, beregata_texture_color,beregata_texture_alpha;

//metode
public:
	
	//constructor .. e apelat cand e instantiata clasa
	Laborator(){
		//setari pentru desenare, clear color seteaza culoarea de clear pentru ecran (format R,G,B,A)
		glClearColor(0.5,0.5,0.5,1);
		glClearDepth(1);			//clear depth si depth test (nu le studiem momentan, dar avem nevoie de ele!)
		glEnable(GL_DEPTH_TEST);	//sunt folosite pentru a determina obiectele cele mai apropiate de camera (la curs: algoritmul pictorului, algoritmul zbuffer)
			 
		//incarca un shader din fisiere si gaseste locatiile matricilor relativ la programul creat
		gl_program_shader1 = lab::loadShader("shadere\\shader_vertex_morocanos.glsl", "shadere\\shader_fragment_morocanos.glsl");
		gl_program_shader2 = lab::loadShader("shader_vertex_fix.glsl", "shader_fragment_fix.glsl");
		gl_program_shader3 = lab::loadShader("shader_vertex_zambaret.glsl", "shader_fragment_zambaret.glsl");
		gl_program_shader4 = lab::loadShader("shader_vertex_particle.glsl", "shader_fragment_particle.glsl");
		//dinti_sus
		lab::loadObj("MY_FACE\\dinti_sus.obj", dinti_sus_vao, dinti_sus_vbo, dinti_sus_ibo, dinti_sus_num_indices);
		dinti_sus_texture_color =  lab::loadTextureBMP("MY_FACE\\dinti_sus.bmp");
		model_matrix_dinti_sus = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);

		//dinti_jos
		lab::loadObj("MY_FACE\\dinti_jos.obj", dinti_jos_vao, dinti_jos_vbo, dinti_jos_ibo, dinti_jos_num_indices);
		dinti_jos_texture_color =  lab::loadTextureBMP("MY_FACE\\dinti_jos.bmp");
		model_matrix_dinti_jos = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);

		//limba
		lab::loadObj("MY_FACE\\limba.obj", limba_vao, limba_vbo, limba_ibo, limba_num_indices);
		limba_texture_color =  lab::loadTextureBMP("MY_FACE\\limba.bmp");
		model_matrix_limba = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);

		//beregata
		lab::loadObj("MY_FACE\\beregata.obj", beregata_vao, beregata_vbo, beregata_ibo, beregata_num_indices);
		beregata_texture_color =  lab::loadTextureBMP("MY_FACE\\beregata.bmp");
		model_matrix_beregata = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
		//matrici de modelare si vizualizare
		view_matrix = glm::lookAt(glm::vec3(0,0,40), glm::vec3(0,0,0), glm::vec3(0,1,0));

		//ochi_drept
		lab::loadObj("MY_FACE\\ochi_drept.obj", ochi_drept_vao, ochi_drept_vbo, ochi_drept_ibo, ochi_drept_num_indices);
		ochi_drept_texture_color =  lab::loadTextureBMP("MY_FACE\\ochi_drept.bmp");
		model_matrix_ochi_drept = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
		lab::_loadObjFile("MY_FACE\\ochi_drept.obj",ochi_drept_vertices,ochi_drept_indices);

		//ochi_stang
		lab::loadObj("MY_FACE\\ochi_stang.obj",ochi_stang_vao, ochi_stang_vbo, ochi_stang_ibo, ochi_stang_num_indices);	
		ochi_stang_texture_color = lab::loadTextureBMP("MY_FACE\\ochi_stang.bmp");
		model_matrix_ochi_stang = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
		lab::_loadObjFile("MY_FACE\\ochi_stang.obj",ochi_stang_vertices,ochi_stang_indices);

		// 3 fete
		lab::loadObj("MY_FACE\\my_face.obj",fata_vao, fata_vbo, fata_ibo, fata_num_indices);	
		fata_texture = lab::loadTextureBMP("MY_FACE\\fata.bmp");
		model_matrix_fata = glm::mat4(1,0,0,0,  0,1,0,0, 0,0,1,0, 0,0,0,1);
		lab::_loadObjFile("MY_FACE\\my_face.obj",face_vertices,face_normala_indices);

		lab::loadObj("MY_FACE\\my_angry.obj",fata_incruntata_vao, fata_incruntata_vbo, fata_incruntata_ibo, fata_incruntata_num_indices);	
		fata_incruntata_texture = lab::loadTextureBMP("MY_FACE\\fata.bmp");
		model_matrix_fata = glm::mat4(1,0,0,0,  0,1,0,0, 0,0,1,0, 0,0,0,1);
		lab::_loadObjFile("MY_FACE\\my_angry.obj",face_vertices,face_incruntata_indices);
	
		lab::loadObj("MY_FACE\\my_happy.obj",fata_vesela_vao, fata_vesela_vbo, fata_vesela_ibo, fata_vesela_num_indices);	
		fata_vesela_texture = lab::loadTextureBMP("MY_FACE\\fata.bmp");
		model_matrix_fata = glm::mat4(1,0,0,0,  0,1,0,0, 0,0,1,0, 0,0,0,1);
		lab::_loadObjFile("MY_FACE\\my_happy.obj",face_vertices,face_vesela_indices);
		
		
		  particula_texture =  lab::loadTextureBMP("circle.bmp");
			vertices_particle.push_back(glm::vec3( -5,5,0));
			vertices_particle.push_back(glm::vec3( -5,-5,0));
			vertices_particle.push_back(glm::vec3( 5,-5,0));
			vertices_particle.push_back(glm::vec3( 5,5,0));
		 
			indices_particle.push_back(0);
			indices_particle.push_back(1);
			indices_particle.push_back(2);
			indices_particle.push_back(1);
			indices_particle.push_back(3);
			indices_particle.push_back(2);

		//vertex buffer object -> un obiect in care tinem vertecsii
		
		glGenVertexArrays(1, &vao_particles);
		glBindVertexArray(vao_particles);
 
		glGenBuffers(1,&vbo_particles);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_particles);
		glBufferData(GL_ARRAY_BUFFER, vertices_particle.size()*sizeof(glm::vec3), &vertices_particle[0], GL_STATIC_DRAW);

		//index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1,&ibo_particles);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_particles);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_particle.size()*sizeof(unsigned int), &indices_particle[0], GL_STATIC_DRAW);

		//desenare wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//destructor .. e apelat cand e distrusa clasa
	~Laborator(){
		//distruge shader
		glDeleteProgram(gl_program_shader1);
		glDeleteProgram(gl_program_shader2);
		glDeleteProgram(gl_program_shader3);
		glDeleteProgram(gl_program_shader4);
		//distruge obiecte
		glDeleteBuffers(1,&fata_vesela_vbo);	glDeleteBuffers(1,&fata_vesela_ibo);	glDeleteVertexArrays(1,&fata_vesela_vao);
		glDeleteBuffers(1,&fata_incruntata_vbo);	glDeleteBuffers(1,&fata_incruntata_ibo);	glDeleteVertexArrays(1,&fata_incruntata_vao);
		glDeleteBuffers(1,&fata_vbo);	glDeleteBuffers(1,&fata_ibo);	glDeleteVertexArrays(1,&fata_vao);		glDeleteTextures(1, &fata_texture);
		glDeleteBuffers(1,&ochi_stang_vbo);	glDeleteBuffers(1,&ochi_stang_ibo);	glDeleteVertexArrays(1,&ochi_stang_vao);	glDeleteTextures(1, &ochi_stang_texture_alpha);		glDeleteTextures(1, &ochi_stang_texture_color);
		glDeleteBuffers(1,&ochi_drept_vbo);	glDeleteBuffers(1,&ochi_drept_ibo);	glDeleteVertexArrays(1,&ochi_drept_vao);	glDeleteTextures(1, &ochi_drept_texture_alpha);	    glDeleteTextures(1, &ochi_drept_texture_color);
		glDeleteBuffers(1,&dinti_sus_vbo);	glDeleteBuffers(1,&dinti_sus_ibo);	glDeleteVertexArrays(1,&dinti_sus_vao);	glDeleteTextures(1, &dinti_sus_texture_alpha);	    glDeleteTextures(1, &dinti_sus_texture_color);
		glDeleteBuffers(1,&dinti_jos_vbo);	glDeleteBuffers(1,&dinti_jos_ibo);	glDeleteVertexArrays(1,&dinti_jos_vao);	glDeleteTextures(1, &dinti_jos_texture_alpha);	    glDeleteTextures(1, &dinti_jos_texture_color);
		glDeleteBuffers(1,&limba_vbo);	glDeleteBuffers(1,&limba_ibo);	glDeleteVertexArrays(1,&limba_vao);	glDeleteTextures(1, &limba_texture_alpha);	    glDeleteTextures(1, &limba_texture_color);
		glDeleteBuffers(1,&beregata_vbo);	glDeleteBuffers(1,&beregata_ibo);	glDeleteVertexArrays(1,&beregata_vao);	glDeleteTextures(1, &beregata_texture_alpha);	    glDeleteTextures(1, &beregata_texture_color);
	}

	
	//--------------------------------------------------------------------------------------------
	//functii de cadru ---------------------------------------------------------------------------

	//functie chemata inainte de a incepe cadrul de desenare, o folosim ca sa updatam situatia scenei ( modelam/simulam scena)
	void notifyBeginFrame(){
		//rotatie
		static float angle=0;
		angle = 0.1f;
		model_matrix_fata = glm::rotate(model_matrix_fata,0.0f,glm::vec3(0,1,0));


	}
	//functia de afisare (lucram cu banda grafica)
	void notifyDisplayFrame(){
		
		//pe tot ecranul
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//foloseste shaderul de particule
		glUseProgram(gl_program_shader4);

		//trimmite variabile uniforme la shader
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader4,"view_matrix"),1,false,glm::value_ptr(view_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader4,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));

		//sistemul de particule
 
/*		int particle_count = 1000;
float vt[1000]; // start times
float vv[1000 * 3]; // start velocities vec3
float t_accum = 0.0f; // start time
int j = 0;
for (int i = 0; i < particle_count; i++) {
  // start times
  vt[i] = t_accum;
  t_accum += 0.01f; // spacing for start times is 0.01 seconds
  // start velocities. randomly vary x and z components
  float randx = ((float)rand() / (float)RAND_MAX) * 1.0f - 0.5f;
  float randz = ((float)rand() / (float)RAND_MAX) * 1.0f - 0.5f;
  vv[j] = randx; // x
  vv[j + 1] = 1.0f; // y
  vv[j + 2] = randz; // z
  j+= 3;
}

//cream 2 vbo pt timp si viteza
		unsigned int timp_vbo = 0;
		glGenBuffers(1,&timp_vbo);
		glBindBuffer(GL_ARRAY_BUFFER,timp_vbo);
		glBufferData(GL_ARRAY_BUFFER,particle_count * sizeof(float),&vt,GL_STATIC_DRAW);
		unsigned int timp_pos = glGetAttribLocation(gl_program_shader4, "elapsed_system_time");
		glBindAttribLocation(gl_program_shader4,0,"elapsed_system_time");
		glEnableVertexAttribArray(timp_pos);
		glVertexAttribPointer(timp_pos,1,GL_FLOAT,GL_FALSE,sizeof(float),(void*)0);

		unsigned int viteza_vbo = 0;
		glGenBuffers(1,&viteza_vbo);
		glBindBuffer(GL_ARRAY_BUFFER,viteza_vbo);
		glBufferData(GL_ARRAY_BUFFER,3*particle_count * sizeof(float),&vv,GL_STATIC_DRAW);
		unsigned int viteza_pos = glGetAttribLocation(gl_program_shader4, "v_i");
		glBindAttribLocation(gl_program_shader4,1,"v_i");
		glEnableVertexAttribArray(viteza_pos);
		glVertexAttribPointer(viteza_pos,3,GL_FLOAT,GL_FALSE,sizeof(float),(void*)0);

	//avem 2 vertex buffer-uri ce le vom trimite ca input vertex shaderului
	//vom folosi un singur vao pt a le trimite
		glBindBuffer(GL_ARRAY_BUFFER,fata_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_normala_indices.size()*sizeof(unsigned int), &face_normala_indices[0], GL_STATIC_DRAW);
		unsigned int fata_normala_pos = glGetAttribLocation(gl_program_shader1, "in_position1");
		glBindAttribLocation(gl_program_shader1,0,"in_position1");
		glEnableVertexAttribArray(fata_normala_pos);
		glVertexAttribPointer(fata_normala_pos,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)0);
 
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader4,"model_matrix"),1,false,glm::value_ptr(model_matrix_fata));
		
		glActiveTexture(GL_TEXTURE0+particula_texture);
		glBindTexture(GL_TEXTURE_2D, particula_texture);
		unsigned int locatie3 = glGetUniformLocation(gl_program_shader4, "textura1");
		glUniform1i( locatie3, particula_texture); 

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(glm::vec3),(void*)0);						//trimite pozitii pe pipe 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(glm::vec3),(void*)(sizeof(float)*3));		//trimite normale pe pipe 1
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(glm::vec3),(void*)(2*sizeof(float)*3));	//trimite texcoords pe pipe 2

		glBindVertexArray(vao_particles);
		glDrawElements(GL_TRIANGLES, indices_particle.size(), GL_UNSIGNED_INT, 0);
  */

	if(stare==1)//se incrunta
	{
		//foloseste shaderul
		glUseProgram(gl_program_shader1);
			
		//trimite variabile uniforme la shader
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader1,"view_matrix"),1,false,glm::value_ptr(view_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader1,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));
 
		//aici facem animatia:momentan se incrunta
		GLint angry_w = glGetUniformLocation(gl_program_shader1, "angry_w");
		GLint happy_w = glGetUniformLocation(gl_program_shader1, "happy_w");
		GLint stare_w = glGetUniformLocation(gl_program_shader1, "stare_w");
			angry+=0.02;
			happy=0;
			glUniform1f(angry_w,(sin(angry+3*PI/2)+1)/2);

			
		

		//fata----------------------------
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader1,"model_matrix"),1,false,glm::value_ptr(model_matrix_fata));
		//TODO	
		
		/*comentariul pt fata
		Trebuie sa trimit vertex shaderului 2 vbo cu fata incruntata si fata normala
		iar acesta sa intoarca o fata cu un anumit procent de modificare in functie de 
		o variabila intre 0-1
		Vertex shader trebuie sa faca o interpolare intre fata_incruntata_vao si fata_vao
		*/
		glActiveTexture(GL_TEXTURE0+fata_texture);
		glBindTexture(GL_TEXTURE_2D, fata_texture);
		unsigned int locatie3 = glGetUniformLocation(gl_program_shader1, "textura1");
		glUniform1i( locatie3, fata_texture); 
		
		
		//aici se intampla toata magia
		//trimite vao shaderului	
		
		//pentru fata normala
		glBindBuffer(GL_ARRAY_BUFFER,fata_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_normala_indices.size()*sizeof(unsigned int), &face_normala_indices[0], GL_STATIC_DRAW);
		unsigned int fata_normala_pos = glGetAttribLocation(gl_program_shader1, "in_position1");
		glBindAttribLocation(gl_program_shader1,0,"in_position1");
		glEnableVertexAttribArray(fata_normala_pos);
		glVertexAttribPointer(fata_normala_pos,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)0);
		unsigned int fata_normala_tex = glGetAttribLocation(gl_program_shader1, "in_texcoord1");
		glBindAttribLocation(gl_program_shader1,1,"in_texcoord1");
		glEnableVertexAttribArray(fata_normala_tex);
		glVertexAttribPointer(fata_normala_tex,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)(sizeof(float)*3));

		//pentru fata incruntata
		glBindBuffer(GL_ARRAY_BUFFER,fata_incruntata_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_normala_indices.size()*sizeof(unsigned int), &face_normala_indices[0], GL_STATIC_DRAW);
		unsigned int fata_incruntata_pos = glGetAttribLocation(gl_program_shader1, "in_position2");
		glBindAttribLocation(gl_program_shader1,3,"in_position2");
		glEnableVertexAttribArray(fata_incruntata_pos);
		glVertexAttribPointer(fata_incruntata_pos,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)0);
		unsigned int fata_incruntata_tex = glGetAttribLocation(gl_program_shader1, "in_texcoord1");
		glBindAttribLocation(gl_program_shader1,4,"in_texcoord1");
		glEnableVertexAttribArray(fata_incruntata_tex);
		glVertexAttribPointer(fata_incruntata_tex,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)(sizeof(float)*3));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)(2*sizeof(float)*3));//trimitem normalele pe pipe 2
		glDrawElements(GL_TRIANGLES, fata_num_indices, GL_UNSIGNED_INT, 0);
		
		glDisableVertexAttribArray(fata_normala_pos);
		glDisableVertexAttribArray(fata_incruntata_pos);
		glDisableVertexAttribArray(fata_normala_tex);
		glDisableVertexAttribArray(fata_incruntata_tex);
	
				//foloseste shaderul
		glUseProgram(gl_program_shader2);
			
		//trimite variabile uniforme la shader
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader2,"view_matrix"),1,false,glm::value_ptr(view_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader2,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));

		//ochi_stang----------------------------
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader1,"model_matrix"),1,false,glm::value_ptr(model_matrix_ochi_stang));
		//TODO
 
		glActiveTexture(GL_TEXTURE0+ochi_stang_texture_color);
		glBindTexture(GL_TEXTURE_2D, ochi_stang_texture_color);
		glUniform1i( locatie3, ochi_stang_texture_color); 
		
		glBindVertexArray(ochi_stang_vao);
		glDrawElements(GL_TRIANGLES, ochi_stang_num_indices, GL_UNSIGNED_INT, 0);

		//ochi_drept----------------------------
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader1,"model_matrix"),1,false,glm::value_ptr(model_matrix_ochi_drept));
		//TODO
 
		glActiveTexture(GL_TEXTURE0+ochi_drept_texture_color);
		glBindTexture(GL_TEXTURE_2D, ochi_drept_texture_color);
		glUniform1i( locatie3, ochi_drept_texture_color); 
		
		glBindVertexArray(ochi_drept_vao);
		glDrawElements(GL_TRIANGLES, ochi_drept_num_indices, GL_UNSIGNED_INT, 0);
 
		//dinti_sus----------------------------
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader1,"model_matrix"),1,false,glm::value_ptr(model_matrix_dinti_sus));
		//TODO

		glActiveTexture(GL_TEXTURE0+dinti_sus_texture_alpha);
		glBindTexture(GL_TEXTURE_2D, dinti_sus_texture_alpha);
 

		glActiveTexture(GL_TEXTURE0+dinti_sus_texture_color);
		glBindTexture(GL_TEXTURE_2D, dinti_sus_texture_color);
		glUniform1i( locatie3, dinti_sus_texture_color); 


		glBindVertexArray(dinti_sus_vao);
		glDrawElements(GL_TRIANGLES, dinti_sus_num_indices, GL_UNSIGNED_INT, 0);
 
		//dinti_jos----------------------------
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader1,"model_matrix"),1,false,glm::value_ptr(model_matrix_dinti_sus));
		//TODO

		glActiveTexture(GL_TEXTURE0+dinti_jos_texture_alpha);
		glBindTexture(GL_TEXTURE_2D, dinti_jos_texture_alpha);
	 

		glActiveTexture(GL_TEXTURE0+dinti_jos_texture_color);
		glBindTexture(GL_TEXTURE_2D, dinti_jos_texture_color);
		glUniform1i( locatie3, dinti_jos_texture_color); 

		glBindVertexArray(dinti_jos_vao);
		glDrawElements(GL_TRIANGLES, dinti_jos_num_indices, GL_UNSIGNED_INT, 0);

		//dinti_limba----------------------------
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader1,"model_matrix"),1,false,glm::value_ptr(model_matrix_limba));
		//TODO

		glActiveTexture(GL_TEXTURE0+limba_texture_alpha);
		glBindTexture(GL_TEXTURE_2D, limba_texture_alpha);
		 

		glActiveTexture(GL_TEXTURE0+limba_texture_color);
		glBindTexture(GL_TEXTURE_2D, limba_texture_color);
		glUniform1i( locatie3, limba_texture_color); 

		glBindVertexArray(limba_vao);
		glDrawElements(GL_TRIANGLES, limba_num_indices, GL_UNSIGNED_INT, 0);
	}
	if(stare==0)//se bucura	
	{
		//foloseste shaderul
		glUseProgram(gl_program_shader3);
			
		//trimite variabile uniforme la shader
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader3,"view_matrix"),1,false,glm::value_ptr(view_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader3,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));
 
		//aici facem animatia:momentan se incrunta
		GLint happy_w = glGetUniformLocation(gl_program_shader3, "happy_w");

			happy+=0.02;
			angry=0;
			glUniform1f(happy_w,(sin(happy+3*PI/2)+1)/2);
			
		

		//fata----------------------------
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader3,"model_matrix"),1,false,glm::value_ptr(model_matrix_fata));
		//TODO	
		
		/*comentariul pt fata
		Trebuie sa trimit vertex shaderului 2 vbo cu fata incruntata si fata normala
		iar acesta sa intoarca o fata cu un anumit procent de modificare in functie de 
		o variabila intre 0-1
		Vertex shader trebuie sa faca o interpolare intre fata_incruntata_vao si fata_vao
		*/
		glActiveTexture(GL_TEXTURE0+fata_texture);
		glBindTexture(GL_TEXTURE_2D, fata_texture);
		unsigned int locatie3 = glGetUniformLocation(gl_program_shader3, "textura1");
		glUniform1i( locatie3, fata_texture); 
		
		
		//aici se intampla toata magia
		//trimite vao shaderului	
		
		//pentru fata normala
		glBindBuffer(GL_ARRAY_BUFFER,fata_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_normala_indices.size()*sizeof(unsigned int), &face_normala_indices[0], GL_STATIC_DRAW);
		unsigned int fata_normala_pos = glGetAttribLocation(gl_program_shader3, "in_position1");
		glBindAttribLocation(gl_program_shader3,0,"in_position1");
		glEnableVertexAttribArray(fata_normala_pos);
		glVertexAttribPointer(fata_normala_pos,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)0);
		unsigned int fata_normala_tex = glGetAttribLocation(gl_program_shader3, "in_texcoord1");
		glBindAttribLocation(gl_program_shader3,1,"in_texcoord1");
		glEnableVertexAttribArray(fata_normala_tex);
		glVertexAttribPointer(fata_normala_tex,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)(sizeof(float)*3));

		//pentru fata vesela
		glBindBuffer(GL_ARRAY_BUFFER,fata_vesela_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_normala_indices.size()*sizeof(unsigned int), &face_normala_indices[0], GL_STATIC_DRAW);
		unsigned int fata_vesela_pos = glGetAttribLocation(gl_program_shader3, "in_position3");
		glBindAttribLocation(gl_program_shader3,3,"in_position3");
		glEnableVertexAttribArray(fata_vesela_pos);
		glVertexAttribPointer(fata_vesela_pos,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)0);
		unsigned int fata_vesela_tex = glGetAttribLocation(gl_program_shader3, "in_texcoord1");
		glBindAttribLocation(gl_program_shader3,4,"in_texcoord1");
		glEnableVertexAttribArray(fata_vesela_tex);
		glVertexAttribPointer(fata_vesela_tex,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)(sizeof(float)*3));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)(2*sizeof(float)*3));//trimitem normalele pe pipe 2
		glDrawElements(GL_TRIANGLES, fata_num_indices, GL_UNSIGNED_INT, 0);
		
		glDisableVertexAttribArray(fata_normala_pos);
		glDisableVertexAttribArray(fata_vesela_pos);
		glDisableVertexAttribArray(fata_normala_tex);
		glDisableVertexAttribArray(fata_vesela_tex);

				//foloseste shaderul
		glUseProgram(gl_program_shader2);
			
		//trimite variabile uniforme la shader
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader2,"view_matrix"),1,false,glm::value_ptr(view_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader2,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));

		//ochi_stang----------------------------
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader1,"model_matrix"),1,false,glm::value_ptr(model_matrix_ochi_stang));
		//TODO
 
		glActiveTexture(GL_TEXTURE0+ochi_stang_texture_color);
		glBindTexture(GL_TEXTURE_2D, ochi_stang_texture_color);
		glUniform1i( locatie3, ochi_stang_texture_color); 
		
		glBindVertexArray(ochi_stang_vao);
		glDrawElements(GL_TRIANGLES, ochi_stang_num_indices, GL_UNSIGNED_INT, 0);

		//ochi_drept----------------------------
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader1,"model_matrix"),1,false,glm::value_ptr(model_matrix_ochi_drept));
		//TODO
 
		glActiveTexture(GL_TEXTURE0+ochi_drept_texture_color);
		glBindTexture(GL_TEXTURE_2D, ochi_drept_texture_color);
		glUniform1i( locatie3, ochi_drept_texture_color); 
		
		glBindVertexArray(ochi_drept_vao);
		glDrawElements(GL_TRIANGLES, ochi_drept_num_indices, GL_UNSIGNED_INT, 0);
 
		//dinti_sus----------------------------
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader1,"model_matrix"),1,false,glm::value_ptr(model_matrix_dinti_sus));
		//TODO

		glActiveTexture(GL_TEXTURE0+dinti_sus_texture_alpha);
		glBindTexture(GL_TEXTURE_2D, dinti_sus_texture_alpha);
 

		glActiveTexture(GL_TEXTURE0+dinti_sus_texture_color);
		glBindTexture(GL_TEXTURE_2D, dinti_sus_texture_color);
		glUniform1i( locatie3, dinti_sus_texture_color); 


		glBindVertexArray(dinti_sus_vao);
		glDrawElements(GL_TRIANGLES, dinti_sus_num_indices, GL_UNSIGNED_INT, 0);
 
		//dinti_jos----------------------------
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader1,"model_matrix"),1,false,glm::value_ptr(model_matrix_dinti_sus));
		//TODO

		glActiveTexture(GL_TEXTURE0+dinti_jos_texture_alpha);
		glBindTexture(GL_TEXTURE_2D, dinti_jos_texture_alpha);
	 

		glActiveTexture(GL_TEXTURE0+dinti_jos_texture_color);
		glBindTexture(GL_TEXTURE_2D, dinti_jos_texture_color);
		glUniform1i( locatie3, dinti_jos_texture_color); 

		glBindVertexArray(dinti_jos_vao);
		glDrawElements(GL_TRIANGLES, dinti_jos_num_indices, GL_UNSIGNED_INT, 0);

		//dinti_limba----------------------------
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader1,"model_matrix"),1,false,glm::value_ptr(model_matrix_limba));
		//TODO

		glActiveTexture(GL_TEXTURE0+limba_texture_alpha);
		glBindTexture(GL_TEXTURE_2D, limba_texture_alpha);
		 

		glActiveTexture(GL_TEXTURE0+limba_texture_color);
		glBindTexture(GL_TEXTURE_2D, limba_texture_color);
		glUniform1i( locatie3, limba_texture_color); 

		glBindVertexArray(limba_vao);
		glDrawElements(GL_TRIANGLES, limba_num_indices, GL_UNSIGNED_INT, 0);
	}
		 
	}
	//functie chemata dupa ce am terminat cadrul de desenare (poate fi folosita pt modelare/simulare)
	void notifyEndFrame(){
		 }
	//functei care e chemata cand se schimba dimensiunea ferestrei initiale
	void notifyReshape(int width, int height, int previos_width, int previous_height){
		//reshape
		if(height==0) height=1;
		float aspect = (float)width / (float)height;
		projection_matrix = glm::perspective(75.0f, aspect,0.1f, 10000.0f);
	}


	//--------------------------------------------------------------------------------------------
	//functii de input output --------------------------------------------------------------------
	
	//tasta apasata
	void notifyKeyPressed(unsigned char key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == 27) lab::glut::close();	//ESC inchide glut si 
		if(key_pressed == 32) {
			//SPACE reincarca shaderul si recalculeaza locatiile (offseti/pointeri)
			glDeleteProgram(gl_program_shader1);
			gl_program_shader1 = lab::loadShader("shadere\\shader_vertex_morocanos.glsl", "shadere\\shader_fragment_morocanos.glsl");
		}
		if(key_pressed == 'w'){
			static bool wire =true;
			wire=!wire;
			glPolygonMode(GL_FRONT_AND_BACK, (wire?GL_LINE:GL_FILL));
		}
		if(key_pressed == 'a')
			stare=0;
		if(key_pressed == 'd')
			stare=1;
		if(key_pressed == 's')
			stare=2;
	}
	//tasta ridicata
	void notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y){	}
	//tasta speciala (up/down/F1/F2..) apasata
	void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == GLUT_KEY_F1) lab::glut::enterFullscreen();
		if(key_pressed == GLUT_KEY_F2) lab::glut::exitFullscreen();
	}
	//tasta speciala ridicata
	void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y){}
	//drag cu mouse-ul
	void notifyMouseDrag(int mouse_x, int mouse_y){ }
	//am miscat mouseul (fara sa apas vreun buton)
	void notifyMouseMove(int mouse_x, int mouse_y){ }
	//am apasat pe un boton
	void notifyMouseClick(int button, int state, int mouse_x, int mouse_y){ }
	//scroll cu mouse-ul
	void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y){ }

};


int main(){
	 
	//initializeaza GLUT (fereastra + input + context OpenGL)
	lab::glut::WindowInfo window(std::string("angry head"),1700,1290,100,100,true);
	lab::glut::ContextInfo context(3,3,false);
	lab::glut::FramebufferInfo framebuffer(true,true,true,true);
	lab::glut::init(window,context, framebuffer);

	//initializeaza GLEW (ne incarca functiile openGL, altfel ar trebui sa facem asta manual!)
	glewExperimental = true;
	glewInit();
	std::cout<<"GLEW:initializare"<<std::endl;

	//creem clasa noastra si o punem sa asculte evenimentele de la GLUT
	//DUPA GLEW!!! ca sa avem functiile de OpenGL incarcate inainte sa ii fie apelat constructorul (care creeaza obiecte OpenGL)
	Laborator mylab;
	lab::glut::setListener(&mylab);
 
		//run
	 

	//taste
	std::cout<<"Taste:"<<std::endl<<"\tESC ... iesire"<<std::endl<<"\tSPACE ... reincarca shadere"<<std::endl<<"\tw ... toggle wireframe"<<std::endl;
	 
		
	 lab::glut::run();

	
	return 0;
}