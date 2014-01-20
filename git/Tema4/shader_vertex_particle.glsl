/* shader to update a particle system based on a simple kinematics function */
#version 400
 
in vec3 v_i; // initial velocity
in float start_time;
 
// view and projection matrices
uniform mat4 V, P;

// emitter position in world coordinates
uniform vec3 emitter_pos_wor;
uniform float elapsed_system_time; // system time in seconds

// the fragment shader can use this for it's output colour's alpha component 
out float opacity;
 
void main () {
  // work out the elapsed time for _this particle_ after its start time
  float t = elapsed_system_time - start_time;
  opacity = 0.0;
 
  vec3 p = emitter_pos_wor;
  // particle stays put until it has reached its birth second
  if (t >= 0.0) {
    // gravity
    vec3 a = vec3 (0.0, -1.0, 0.0);
    // this is a standard kinematics equation of motion with velocity and
    // acceleration (gravity)
    p += v_i * t + 0.5 * a * t * t;
    // gradually make particle fade to invisible over 3 seconds
    opacity = 1.0 - (t / 3.0);
  }
  gl_Position = P * V * vec4 (p, 1.0);
  gl_PointSize = 15.0; // size in pixels
}