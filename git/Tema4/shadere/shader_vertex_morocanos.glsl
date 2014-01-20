#version 330

attribute vec3 in_position1;	
attribute vec3 in_position2; 
attribute vec3 in_position3;


attribute vec2 in_texcoord1;
attribute vec2 in_texcoord2;

uniform mat4 model_matrix, view_matrix, projection_matrix;

out vec2 texcoord;

varying vec3 n_eye;
varying vec3 pos_eye;

varying vec3 Lp_eye;
vec3 in_position4;
vec3 in_position5;
uniform float happy_w;
uniform float angry_w;
uniform float stare_w;
void main(){

	texcoord =  in_texcoord1 ;
 

	    in_position4 = mix( in_position1 , in_position2,angry_w) ;
		in_position5 = mix( in_position1 , in_position3,angry_w) ;
 
		pos_eye = (view_matrix * model_matrix * vec4(in_position4,1.0)).xyz;
	 
	
	Lp_eye = (view_matrix * vec4(1.0,1.0,10.0,1.0)).xyz;
	 
 
		gl_Position = projection_matrix*view_matrix*model_matrix*vec4(in_position4,1); 
	 
	
}
