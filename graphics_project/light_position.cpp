#include <light_position.h>

void lightPositionUpdate(vec3 & lightPosDir, vec3 & lightPos, bool & lightPosToggle){
	// Updating Light Position in X
	if (lightPosToggle == true) {
		if (lightPos.v[1] >= 70.0f) {
			lightPosDir.v[1] = 1;
		}
		else if (lightPos.v[1] <= -70.0f) {
			lightPosDir.v[1] = 0;
		}
		if (lightPosDir.v[1] == 0) {
			lightPos.v[1] = lightPos.v[1] + 0.025;
		}
		else if (lightPosDir.v[1] == 1) {
			lightPos.v[1] = lightPos.v[1] - 0.025;
		}
	}

}