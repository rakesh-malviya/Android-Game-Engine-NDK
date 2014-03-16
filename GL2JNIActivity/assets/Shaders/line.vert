attribute vec4 vPosition;
varying vec4 vFinalColor;
uniform mat4 projMatrix;
uniform mat4 modelMatrix;
uniform vec4 uColor;

void main()
{
	gl_Position = projMatrix*modelMatrix*vPosition;
	vFinalColor = uColor;
}
