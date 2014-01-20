/* shader to render simple particle system points */
#version 400

in float opacity;
uniform sampler2D texture1; // optional. enable point-sprite coords to use
out vec4 frag_colour;

const vec4 particle_colour = vec4 (0.1, 0.1, 0.8, 0.8); 

void main () {
  // using point texture coordinates which are pre-defined over the point
  vec4 texel = texture (texture1, gl_PointCoord);
  frag_colour.rgb = particle_colour.rgb * texel.rgb;
  frag_colour.a = opacity * texel.a;
}