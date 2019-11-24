#include "keyboard_mouse.h"

void keyPress(unsigned char key, int xmouse, int ymouse) {
	//std::cout << "Keypress: " << keypress << std::endl;
	switch(key){
		// Main object movement
		case('w'):
			y_pos -= 0.02f;
			break;
		case('a'):
			x_pos -= 0.02f;
			break;
		case('s'):
			y_pos += 0.02f;
			break;
		case('d'):
			x_pos += 0.02f;
			break;
		case('f'):
			z_mouse = -z_mouse;
			break;
		case('r'):
			modelRotationToggle = !modelRotationToggle;
			break;
		case('l'):
			lightPositionToggle = !lightPositionToggle;
			break;
		case('h'):
			ambientStrength = ambientStrength + 1;
			if (ambientStrength > 2) {
				ambientStrength = 0;
			}
			std::cout << ambientStrength << std::endl;
			break;
		// Reset Condition
		case('R'):
			z_mouse = 0.3f;
			x_pos = 0.0f;
			y_pos = -0.06f;
			z_pos = 0.0f;
			ambientStrength = 1.0f;
			specularStrength = 1.0f;
			modelRotationToggle = false;
			modelRotation = 0.0f;
			lightPositionDirection = vec3(0, 0, 0);
			lightPosition = vec3(0, 70, 50);
			lightPositionToggle = false;
			break;
	}
};

void mouseMove(int x, int y) {
	// Fix x_mouse coordinates calculation to only take the first viewport
	x_mouse = (float)-(x - width / 2) / (width / 2);
	y_mouse = (float)-(y - height / 2) / (height / 2);
};

void mouseWheel(int key, int wheeldir, int x, int y) {
	if (wheeldir ==  1)
	{
		z_mouse -= 0.2f;
	} else if (wheeldir == -1){
		z_mouse += 0.2f;
	}
}

void specialKeypress(int key, int x, int y) {
	switch (key) {
	// Specular Lighting settings, up for up, down for down
		case(GLUT_KEY_UP):
			specularStrength += 0.02;
			break;
		case(GLUT_KEY_DOWN):
			specularStrength -= 0.02;
			break;
	}
};