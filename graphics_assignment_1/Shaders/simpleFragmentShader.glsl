#version 330

in vec3 nEye;
in vec3 vsNormals;
in vec3 vsPosition;
in vec3 vsLightPos;

out vec4 fragColour;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float ambientStr;
uniform float specularStr;

void main(){
	// Global Lighting Variables
	vec3 lightDir = normalize(lightPos - vsPosition);
	vec3 norm = normalize(vsNormals);
	// Diffuse Lighting
	float diffuse = max(dot(norm, lightDir), 0.0);
	// Specular Lighting
	vec3 viewDir = normalize(viewPos - vsPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); 
	fragColour = vec4 (nEye * ambientStr * diffuse , 1.0);
}