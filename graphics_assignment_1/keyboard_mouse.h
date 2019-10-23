#ifndef KEYBOARD_MOUSE_H_
#define KEYBOARD_MOUSE_H_

// OpenGL Libraries
#include <GL/glew.h>
#include <GL/freeglut.h>

// C++ standard libraries
#include <iostream>


// x_mouse, y_mouse variables for mouse view
extern float x_mouse;
extern float y_mouse;
extern int width;
extern int height;
extern bool model2ToggleView;

void keypress(unsigned char key, int xmouse, int ymouse);

void special_keypress(int key, int x, int y);

void mouse_move(int x, int y);

#endif //KEYBOARD_MOUSE_H_