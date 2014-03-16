precision mediump float;
varying vec4 vFinalColor;
varying vec3 vertex_to_light_vector;
varying vec3 normal;
void main() {
  	const vec4 AmbientColor = vec4(0.1, 0.0, 0.0, 1.0);
  	const vec4 DiffuseColor = vec4(0.9, 0.9, 0.9, 1.0);
	vec3 normalized_normal = normalize(normal);
	vec3 normalized_vertex_to_light_vector = normalize(-vertex_to_light_vector);
	float DiffuseTerm = clamp(dot(normalized_normal, normalized_vertex_to_light_vector), 0.0, 1.0);
	gl_FragColor.x = AmbientColor.x + DiffuseColor.x*DiffuseTerm*vFinalColor.x;
	gl_FragColor.y = AmbientColor.y + DiffuseColor.y*DiffuseTerm*vFinalColor.y;
	gl_FragColor.z = AmbientColor.z + DiffuseColor.z*DiffuseTerm*vFinalColor.z;
	gl_FragColor.w = AmbientColor.w + DiffuseColor.w*DiffuseTerm*vFinalColor.w;
}