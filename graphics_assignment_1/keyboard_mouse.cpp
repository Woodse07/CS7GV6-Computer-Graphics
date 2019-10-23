#include "keyboard_mouse.h"

void keypress(unsigned char key, int xmouse, int ymouse) {
	std::cout << "Keypress" << keypress << std::endl;
};

void mouse_move(int x, int y) {
	// Fix x_mouse coordinates calculation to only take the first viewport
	x_mouse = (float) -(x - width/2) / (width / 2);
	y_mouse = (float) -(y - height/2) / (height/2);
};