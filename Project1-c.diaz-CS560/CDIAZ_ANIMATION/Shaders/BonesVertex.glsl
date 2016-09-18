#version 330

layout (location  = 0 )in vec4 vp;
uniform mat4 camera ,model;

void main () {
	gl_PointSize = 10.0;
	gl_Position = camera * model* vp;
}
