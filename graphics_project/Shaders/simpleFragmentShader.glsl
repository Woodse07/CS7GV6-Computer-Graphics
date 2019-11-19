#version 330

in vec3 fragPos;
in vec3 nEye;
in vec3 vsNormals; // World Normal
in vec3 vsLightPos;
in vec3 vsPosition; // World Position

out vec4 fragColour;

uniform vec3 lightPos, viewPos;

uniform float ambientStr, specularStr;

// Ambient Variables
const vec3 ambientColor = vec3(0.90, 0.0, 0.20);

// Diffuse Variables
const int levels = 5;
const float scaleFactor = 1.0 / levels;

// Specular Variables
const vec3 specColor = vec3(1.0, 1.0, 1.0);

// Material Properties
const int materialShininess = 50;
const float materialKd = 1.0;
const float materialKs = 1.0;

void main() {
	// Global Lighting Variables
	vec3 lightDir = normalize(lightPos - vsPosition);
	vec3 vDir = normalize(viewPos - vsPosition);
	vec3 H = normalize(lightDir + vDir);

	// Ambient Lighting
	vec3 ambient = ambientStr * ambientColor;
	// Diffuse Lighting
	vec3 diffuseColor = vec3(1.0, 0.0, 0.0);
	float diffuse = materialKd * max(0, dot(lightDir, vsNormals));
	diffuseColor = diffuseColor * materialKd * floor(diffuse * levels) * scaleFactor;

	// Specular Lighting
	float specular = 0.0;
	if (dot(lightDir, vsNormals) > 0.0) {
		specular = materialKs * pow(max(0, dot(H, vsNormals)), materialShininess);
	}
	// Limit Specular
	float specMask = (pow(dot(H, vsNormals), materialShininess) > 0.2) ? 1 : 0;

	// Edge Detection
	float edgeDetection = (dot(vDir, vsPosition) > 0.2) ? 1 : 0;
	// Fragment Color
	vec3 color = (diffuseColor + specular*specMask);
	fragColour = vec4(color, 1);
	//fragColour = vec4(viewPos * ambient + diffuse + specular, 1.0);

}