#version 330 core
// FRAGMENT SHADER

out vec4 color;

in float positionVariable;

void main()
{
	color = vec4(positionVariable, 0.5, 0.2, 1.0);
}