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
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 norm = normalize(vsNormals);
	if(ambientStr == 0) {
		float intensity;
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
	}else if (ambientStr == 1) {
		// Diffuse Lighting
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * diffuseColor;
		// Specular Lighting
		vec3 reflectDir = reflect(-lightDir, norm);
		vec3 viewDir = normalize(viewPos - fragPos);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		vec3 specular = spec * specularStr * specColor;
		color = vec4(nEye * ambientColor + diffuse + specular, 1.0);
	} else if (ambientStr == 2) {
		color = vec4(nEye, 1.0);
		
	} else if (ambientStr == 3) {
		color = vec4(0.0, 0.0, 0.0, 1.0);
	}
	fragColour = color;

}