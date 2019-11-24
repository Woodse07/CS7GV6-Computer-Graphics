#version 330

in vec3 fragPos;
in vec3 nEye;
in vec3 vsNormals;
in vec3 vsLightPos;
in vec3 vsPosition;

out vec4 fragColour;

uniform vec3 lightPos, viewPos;

uniform float ambientStr;
uniform float specularStr;

const vec3 ambientColor = vec3(1.0, 0.0, 0.0);
const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
const vec3 specColor = vec3(1.0, 1.0, 1.0);

void main() {
	// Global Lighting Variables
	vec4 color;
	if (ambientStr == 0) {
		color = vec4(0.0, 0.0, 0.0, 1.0);
	}else if(ambientStr == 1) {
		float intensity;
		vec3 lightDir = normalize(lightPos - fragPos);
		vec3 norm = normalize(vsNormals);
		intensity = dot(lightDir, normalize(norm));
		if (intensity > 0.99) {
			color = vec4(1.0, 1.0, 1.0, 1.0);
		}
		else if (intensity > 0.95) {
			color = vec4(1.0, 0.5, 0.5, 1.0);
		}
		else if (intensity > 0.5) {
			color = vec4(0.6, 0.3, 0.3, 1.0);
		}
		else if (intensity > 0.25) {
			color = vec4(0.4, 0.2, 0.2, 1.0);
		}
		else {
			color = vec4(0.2, 0.1, 0.1, 1.0);
		}
	}
	fragColour = color;

}