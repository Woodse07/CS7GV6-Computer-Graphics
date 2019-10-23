#include "keyboard_mouse.h"

void keypress(unsigned char key, int xmouse, int ymouse) {
	//std::cout << "Keypress: " << keypress << std::endl;
};

void special_keypress(int key, int x, int y) {
	//std::cout << "Special Keypress: " << keypress << std::endl;
	if (key == GLUT_KEY_F1) {
		//std::cout << "F1 Pressed, Map Toggle" << std::endl;
		model2ToggleView = !model2ToggleView;
	}
};

void mouse_move(int x, int y) {
	// Fix x_mouse coordinates calculation to only take the first viewport
	x_mouse = (float) -(x - width/2) / (width / 2);
	y_mouse = (float) -(y - height/2) / (height/2);
};