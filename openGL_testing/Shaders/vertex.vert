# version 330 core
// VERTEX SHADER

layout (location = 0) in vec3 aPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float time;

vec3 newPos;

void main(){
	newPos = aPos;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(newPos, 1.0);
}