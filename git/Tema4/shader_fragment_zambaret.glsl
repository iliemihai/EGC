#version 330
layout(location = 0) out vec4 out_color;

uniform sampler2D textura1;
uniform sampler2D textura2;
uniform sampler2D textura3;
uniform int has_alpha;

in vec2 texcoord;

void main(){

	vec3 tex1 = texture(textura1, texcoord).xyz;
	vec3 tex2 = texture(textura2, texcoord).xyz;
	vec3 tex3 = texture(textura3, texcoord).xyz;



	out_color = vec4(tex1, 1);
}