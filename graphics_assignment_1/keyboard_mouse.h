#ifndef KEYBOARD_MOUSE_H_
#define KEYBOARD_MOUSE_H_

// C++ standard libraries
#include <iostream>

// x_mouse, y_mouse variables for mouse view
extern float x_mouse;
extern float y_mouse;
extern int width;
extern int height;

void keypress(unsigned char key, int xmouse, int ymouse);

void mouse_move(int x, int y);

#endif //KEYBOARD_MOUSE_H_