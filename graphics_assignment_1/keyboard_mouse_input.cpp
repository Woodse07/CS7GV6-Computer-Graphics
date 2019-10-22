#include "keyboard_mouse_input.h"

void keypress(unsigned char key, int xmouse, int ymouse) {
	std::cout << "Keypress" << keypress << std::endl;
};

void mouse_move(int x, int y) {
	// Fix x_mouse coordinates calculation to only take the first viewport
	if (x >= width / 2) {
		x_mouse = -1.0f;
	}else {
		x_mouse = (float) -(x - width/4) / (width / 4);
	}
	y_mouse = (float) -(y - height/2) / (height/2);
};