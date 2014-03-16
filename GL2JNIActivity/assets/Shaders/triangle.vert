attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec4 vNormal;
varying vec4 vFinalColor;
varying vec3 vertex_to_light_vector;
varying vec3 normal;
uniform mat4 projMatrix;
uniform mat4 modelMatrix;

void main() {
  	gl_Position = projMatrix*modelMatrix*vPosition;
	normal = vec3(normalize(projMatrix*modelMatrix*vNormal));
	vec4 vertex_in_modelview_space = modelMatrix* vPosition;
	vec4 lightPos = vec4(100, 100.0, 100.0, 1.0);
	vertex_to_light_vector = normalize(vec3(vertex_in_modelview_space - lightPos));
 	vFinalColor = vColor;
}