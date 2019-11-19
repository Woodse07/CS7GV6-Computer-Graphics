#include <light_position.h>

void lightPositionUpdate(vec3 & lightPosDir, vec3 & lightPos) {
	// Updating Light Position in X
	if (lightPos.v[0] >= 200.0f) {
		lightPosDir.v[0] = 1;
	}
	else if (lightPos.v[0] <= -200.0f) {
		lightPosDir.v[0] = 0;
	}
	if (lightPosDir.v[0] == 0) {
		lightPos.v[0] = lightPos.v[2] + 0.04;
	}
	else if (lightPosDir.v[0] == 1) {
		lightPos.v[0] = lightPos.v[0] - 0.04;
	}
	// Updating Light Position in Z
	if (lightPos.v[2] >= 100.0f) {
		lightPosDir.v[2] = 1;
	}
	else if (lightPos.v[2] <= -100.0f) {
		lightPosDir.v[2] = 0;
	}
	if (lightPosDir.v[2] == 0) {
		lightPos.v[2] = lightPos.v[2] + 0.02;
	} else if (lightPosDir.v[2] == 1) {
		lightPos.v[2] = lightPos.v[2] - 0.02;
	}
}