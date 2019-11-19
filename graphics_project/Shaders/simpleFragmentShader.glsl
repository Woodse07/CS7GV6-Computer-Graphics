#version 330

in vec3 fragPos;
in vec3 nEye;
in vec3 vsNormals; // World Normal
in vec3 vsPosition; // World Position

out vec4 fragColour;

uniform vec3 lightPos, viewPos;
uniform mat4 view;

uniform float ambientStr, specularStr;

const vec3 diffuseLight = vec3(0.5, 0.5, 0.5);
const vec3 diffuseK = vec3(0.0, 1.0, 1.0);

vec3 specularLight = vec3(1.0, 1.0, 1.0);
vec3 specularK = vec3(1.0, 1.0, 1.0);

const vec3 ambientColor = vec3(1.0, 0.0, 0.0);
const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
const vec3 specColor = vec3(1.0, 1.0, 1.0);

const int materialShininess = 100;
const float materialKd = 0.5;
const float materialKs = 0.3;



void main(){
	// Global Lighting Variables

	vec3 normal = normalize(nEye);

	vec3 lightPosEye = vec3(view * vec4(lightPos, 1.0));
	vec3 lightDirEye = lightPosEye; normalize(lightPosEye - viewPos);

	// Diffuse Light
	float dotProd = max(dot(lightDirEye, nEye), 0.0);
	dotProd = floor(dotProd * 4) / (4 + 0.25);
	vec3 diffuse = diffuseLight * diffuseK * dotProd;

	vec3 surfaceEye = normalize(-viewPos);
	vec3 halfEye = normalize(surfaceEye + lightDirEye);
	float dotSpec = max(dot(halfEye, nEye), 0.0);
	dotSpec = pow(dotSpec, 50);
	dotSpec = step(0.3, dotSpec);
	vec3 specular = specularLight * specularK * dotSpec;

	fragColour = vec4(specular + diffuse, 1.0);
}