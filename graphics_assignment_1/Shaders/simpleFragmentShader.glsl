#version 330

in vec3 fragPos;
in vec3 nEye;
in vec3 vsNormals;
in vec3 vsLightPos;
in vec3 vsPosition;

out vec4 fragColour;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float ambientStr;
uniform float specularStr;

const vec3 ambientColor = vec3(1.0, 0.0, 0.0);
const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
const vec3 specColor = vec3(1.0, 1.0, 1.0);

void main(){
	// Global Lighting Variables
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 norm = normalize(vsNormals);
	// Ambient Lighting
	vec3 ambient = ambientStr * ambientColor;
	// Diffuse Lighting
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * diffuseColor;
	// Specular Lighting
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(viewPos - fragPos);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); 
	vec3 specular = spec * specularStr * specColor;
	fragColour = vec4(nEye*ambient + diffuse + specular, 1.0);
}