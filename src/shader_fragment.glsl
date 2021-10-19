#version 450 core

in vec4 interpolatedColor;

out vec4 fragmentColor;

void main()
{
	fragmentColor = interpolatedColor;
}
