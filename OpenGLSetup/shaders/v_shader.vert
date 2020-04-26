#version 430

/*uniform mat4 mvMat; 
uniform mat4 projMat; 
*/
//layout(location = 4) in vec4 color_ssbo;
//layout(location = 1) in vec4 vertex_color;

out vec4 color;

void main() {
  color = vec4(1,1,0,1);
  //gl_Position = projMat * mvMat * vertex_position;
}




