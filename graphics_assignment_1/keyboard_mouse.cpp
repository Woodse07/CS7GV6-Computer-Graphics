#include "keyboard_mouse.h"

void keyPress(unsigned char key, int xmouse, int ymouse) {
	//std::cout << "Keypress: " << keypress << std::endl;
	switch(key){
		// Main object movement
		case('w'):
			z_pos -= 0.02f;
			break;
		case('a'):
			x_pos -= 0.02f;
			break;
		case('s'):
			z_pos += 0.02f;
			break;
		case('d'):
			x_pos += 0.02f;
			break;
		// Attack mode or 'Deploy hierarchical transforms'
		case('q'):
			break;
		// Reset Condition
		case('R'):
			z_mouse = 1.0f;
			x_pos = 0.0f;
			z_pos = 0.0f;
			ambientStrength = 1.0f;
			specularStrength = 1.0f;
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
	//std::cout << "Special Keypress: " << keypress << std::endl;
		case(GLUT_KEY_F1):
			model2ToggleView = !model2ToggleView;
			break;
	// Specular Lighting settings, up for up, down for down
		case(GLUT_KEY_UP):
			specularStrength += 0.02;
			break;
		case(GLUT_KEY_DOWN):
			specularStrength -= 0.02;
			break;
	// Ambient Lighting controls, left for down, right for up
		case(GLUT_KEY_LEFT):
			ambientStrength -= 0.02;
			break;
		case(GLUT_KEY_RIGHT):
			ambientStrength += 0.02;
			break;
	}
};