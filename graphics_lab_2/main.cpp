#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <maths_funcs.h>

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;

bool keyArray[127];
mat4 matId = identity_mat4();
mat4 matScale = identity_mat4();
mat4 matTranslate = identity_mat4();
mat4 matRotate = identity_mat4();
mat4 matSTR_1 = identity_mat4();
mat4 matSTR_2 = identity_mat4();
mat4 matSTR_3 = identity_mat4();
GLint uniform_matid_1;
GLint uniform_matid_2;
GLint uniform_matid_3;
bool debug_mode = true;
vec3 vPosition;

// Vertex Shader (for convenience, it is defined in the main here, but we will be using text files for shaders in future)
// Note: Input to this shader is the vertex positions that we specified for the triangle. 
// Note: gl_Position is a special built-in variable that is supposed to contain the vertex position (in X, Y, Z, W)
// Since our triangle vertices were specified as vec3, we just set W to 1.0.
static const char* pVS = "                                           \n\
#version 330                                                         \n\
                                                                     \n\
in vec3 vPosition;													 \n\
in vec4 vColor;														 \n\
in int gl_VertexID;                                                  \n\
out vec4 color;														 \n\
uniform mat4 matScTrRo_1;                                            \n\
uniform mat4 matScTrRo_2;                                            \n\
uniform mat4 matScTrRo_3;                                            \n\
                                                                     \n\
                                                                     \n\
void main()                                                          \n\
{                                                                    \n\
	if(gl_VertexID >= 0 && gl_VertexID <=2){                         \n\
		gl_Position = matScTrRo_1 * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);  \n\
	}else if(gl_VertexID>=3 && gl_VertexID <=5){					 \n\
		gl_Position = matScTrRo_2 * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);  \n\
	}else if(gl_VertexID>=6 && gl_VertexID <=8){					\n\
		gl_Position = matScTrRo_3 * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);  \n\
	}                                                                \n\
	color = vColor;							                         \n\
}";

// Fragment Shader
// Note: no input in this shader, it just outputs the colour of all fragments, in this case set to red (format: R, G, B, A).
static const char* pFS = "  \n\
#version 330                \n\
in vec4 color;				\n\
out vec4 FragColor;			\n\
                            \n\
void main()                 \n\
{                           \n\
    FragColor = color;		\n\
}";

// Shader Functions- click on + to expand
#pragma region SHADER_FUNCTIONS
static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// create a shader object
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }
	// Bind the source code to the shader, this happens before compilation
	glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderText, NULL);
	// compile the shader and check for errors
    glCompileShader(ShaderObj);
    GLint success;
	// check for shader related errors using glGetShaderiv
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }
	// Attach the compiled shader object to the program object
    glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders()
{
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
    GLuint shaderProgramID = glCreateProgram();
    if (shaderProgramID == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

	// Create two shader objects, one for the vertex, and one for the fragment shader
    AddShader(shaderProgramID, pVS, GL_VERTEX_SHADER);
    AddShader(shaderProgramID, pFS, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };


	// After compiling all shader objects and attaching them to the program, we can finally link it
    glLinkProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
    glValidateProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
    glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }
	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
    glUseProgram(shaderProgramID);
	return shaderProgramID;
}
#pragma endregion SHADER_FUNCTIONS

// VBO Functions - click on + to expand
#pragma region VBO_FUNCTIONS
GLuint generateObjectBuffer(GLfloat vertices[], GLfloat colors[], GLint numVertices) {
	// Genderate 1 generic buffer object, called VBO
	GLuint VBO;
 	glGenBuffers(1, &VBO);
	// In OpenGL, we bind (make active) the handle to a target name and then execute commands on that target
	// Buffer will contain an array of vertices 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
	glBufferData(GL_ARRAY_BUFFER, numVertices*7*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	// if you have more data besides vertices (e.g., vertex colours or normals), use glBufferSubData to tell the buffer when the vertices array ends and when the colors start
	glBufferSubData (GL_ARRAY_BUFFER, 0, numVertices*3*sizeof(GLfloat), vertices);
	glBufferSubData (GL_ARRAY_BUFFER, numVertices*3*sizeof(GLfloat), numVertices*4*sizeof(GLfloat), colors);
return VBO;
}

void linkCurrentBuffertoShader(GLuint shaderProgramID, GLint numVertices){
	// find the location of the variables that we will be using in the shader program
	GLuint positionID = glGetAttribLocation(shaderProgramID, "vPosition");
	GLuint colorID = glGetAttribLocation(shaderProgramID, "vColor");
	// Have to enable this
	glEnableVertexAttribArray(positionID);
	// Tell it where to find the position data in the currently active buffer (at index positionID)
    glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Similarly, for the color data.
	glEnableVertexAttribArray(colorID);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numVertices*3*sizeof(GLfloat)));
}
#pragma endregion VBO_FUNCTIONS


void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	// NB: Make the call to draw the geometry in the currently activated vertex buffer. This is where the GPU starts to work!
	glDrawArrays(GL_TRIANGLES, 0, 9);
    glutSwapBuffers();
	glutPostRedisplay();
}

void keyInputs(unsigned char key, int xmouse, int ymouse)
{
	// Toggling options (scaling, translation, rotation)
	switch (key) {
		// Rotation
		case('r'):
			keyArray['r'] = !keyArray['r'];
			if (keyArray['r'] == false){
				std::cout << "Rotation off" << std::endl;
			}else {
				std::cout << "Rotation on, Scale & Translation off!" << std::endl;
				keyArray['s'] = false;
				keyArray['t'] = false;
			}
			break;
		// Translation
		case('t'):
			keyArray['t'] = !keyArray['t'];
			if (keyArray['t'] == false) {
				std::cout << "Translation off" << std::endl;
			}else {
				std::cout << "Translation on, Scaling & Rotation off!" << std::endl;
				keyArray['s'] = false;
				keyArray['r'] = false;
			}
			break;
		// Scaling
		case('s'):
			keyArray['s'] = !keyArray['s'];
			if (keyArray['s'] == false) {
				std::cout << "Scaling off" << std::endl;
			}else {
				std::cout << "Scaling on, Rotation & Translation off!" << std::endl;
				keyArray['r'] = false;
				keyArray['t'] = false;
			}
			break;
		case('x'):
			keyArray['x'] = !keyArray['x'];
			if (keyArray['x'] == false) {
				std::cout << "Scaling on the X axis off" << std::endl;
			}else {
				std::cout << "Scaling on the X axis on" << std::endl;
			}
			break;
		case('y'):
			keyArray['y'] = !keyArray['y'];
			if (keyArray['y'] == false) {
				std::cout << "Scaling on the Y axis off" << std::endl;
			}else {
				std::cout << "Scaling on the Y axis on" << std::endl;
			}
			break;
		case('z'):
			keyArray['z'] = !keyArray['z'];
			if (keyArray['z'] == false) {
				std::cout << "Scaling on the Z axis off" << std::endl;
			}else {
				std::cout << "Scaling on the Z axis on" << std::endl;
			}
			break;
		case('d'):
			debug_mode = !debug_mode;
			if (debug_mode == false) {
				std::cout << "Debug mode off!" << std::endl;
			}else {
				std::cout << "Debug mode on!" << std::endl;
			}
			break;
		case('R'):
			std::cout << "Resetting" << std::endl;
			matScale = identity_mat4();
			matTranslate = identity_mat4();
			matRotate = identity_mat4();
			matSTR_1 = identity_mat4();
			break;
		default:
			break;
	}

	// Scaling, cleaning up matrix
	matScale = identity_mat4();
	if (keyArray['s'] == true) {
		if ((keyArray['x'] && keyArray['y'] && keyArray['z']) == true) {
			switch (key) {
				case('+'):
					std::cout << "Scaling up X, Y, Z\n" << endl;
					matScale = scale(matId, vec3(2.0f, 2.0f, 2.0f));
					break;
				case('-'):
					std::cout << "Scaling down X, Y, Z\n" << endl;
					matScale = scale(matId, vec3(0.5f, 0.5f, 0.5f));
					break;
				default:
					break;
			}
		// X only
		}else if ((keyArray['x'] && !keyArray['y'] && !keyArray['z']) == true) {
			switch (key) {
				case('+'):
					std::cout << "Scaling up X\n" << endl;
					matScale = scale(matId, vec3(2.0f, 1.0f, 1.0f));
					break;
				case('-'):
					std::cout << "Scaling down X\n" << endl;
					matScale = scale(matId, vec3(0.5f, 1.0f, 1.0f));
					break;
				default:
					break;
			}
		// Y only
		}else if ((!keyArray['x'] && keyArray['y'] && !keyArray['z']) == true) {
			switch (key) {
				case('+'):
					std::cout << "Scaling up Y\n" << endl;
					matScale = scale(matId, vec3(1.0f, 2.0f, 1.0f));
					break;
				case('-'):
					std::cout << "Scaling down Y\n" << endl;
					matScale = scale(matId, vec3(1.0f, 0.5f, 1.0f));
					break;
				default:
					break;
			}

		}else if ((!keyArray['x'] && !keyArray['y'] && keyArray['z']) == true) {
			switch (key) {
			case('+'):
				std::cout << "Scaling up Z\n" << endl;
				matScale = scale(matId, vec3(1.0f, 1.0f, 2.0f));
				break;
			case('-'):
				std::cout << "Scaling down Z\n" << endl;
				matScale = scale(matId, vec3(1.0f, 1.0f, 0.5f));
				break;
			default:
				break;
			}

		}
	}
	// Translation, cleaning up matrix
	matTranslate = identity_mat4();
	if (keyArray['t'] == true) {
		switch (key) {
			case('1'):
				std::cout << (debug_mode ? "Translating + X" : "") << std::endl;
				matTranslate.m[12] = 0.02f;
				break;
			case('2'):
				std::cout << (debug_mode ? "Translating - X" : "") << std::endl;
				matTranslate.m[12] = -0.02f;
				break;
			case('3'):
				std::cout << (debug_mode ? "Translating + Y" : "") << std::endl;
				matTranslate.m[13] = 0.02f;
				break;
			case('4'):
				std::cout << (debug_mode ? "Translating - Y" : "") << std::endl;
				matTranslate.m[13] = -0.02f;
				break;
			case('5'):
				std::cout << (debug_mode ? "Translating + Z" : "") << std::endl;
				matTranslate.m[14] = 0.02f;
				break;
			case('6'):
				std::cout << (debug_mode ? "Translating - Z" : "") << std::endl;
				matTranslate.m[14] = -0.02f;
				break;
			default:
				break;
		}
	}
	
	if (keyArray['r'] == true) {
		switch (key) {
			case('1'):
				std::cout << (debug_mode ? "Rotating + X, 1 degree" : "") << std::endl;
				matRotate = rotate_x_deg(matRotate, 1);
				break;
			case('2'):
				std::cout << (debug_mode ? "Rotating - X, 1 degree" : "") << std::endl;
				matRotate = rotate_x_deg(matRotate, -1);
				break;
			case('3'):
				std::cout << (debug_mode ? "Rotating + Y, 1 degree" : "") << std::endl;
				matRotate = rotate_y_deg(matRotate, 1);
				break;
			case('4'):
				std::cout << (debug_mode ? "Rotating - Y, 1 degree" : "") << std::endl;
				matRotate = rotate_y_deg(matRotate, -1);
				break;
			case('5'):
				std::cout << (debug_mode ? "Rotating + Z, 1 degree" : "") << std::endl;
				matRotate = rotate_z_deg(matRotate, 1);
				break;
			case('6'):
				std::cout << (debug_mode ? "Rotating - Z, 1 degree" : "") << std::endl;
				matRotate = rotate_z_deg(matRotate, -1);
				break;
			default:
				break;
		}
	}
	// Rotation
//	matRotate = identity_mat4();
//	if (keyArray['r'] == true) {
//		
//	}
	// Debug messages/prints of matrices
	matSTR_1= (matSTR_1* matScale * matTranslate * matRotate);
	if( debug_mode == true){
		std::cout << "\nScale Matrix";
		print(matScale);
		std::cout << "\nTranslation Matrix";
		print(matTranslate);
		std::cout << "\Rotation Matrix";
		print(matRotate);
		std::cout << "\nSTR Final Matrix";
		print(matSTR_1);
		std::cout << "\n" << endl;
	}
	glUniformMatrix4fv(uniform_matid_1, 1, GL_FALSE, matSTR_1.m);
	glUniformMatrix4fv(uniform_matid_2, 1, GL_FALSE, matSTR_2.m);
	glUniformMatrix4fv(uniform_matid_3, 1, GL_FALSE, matSTR_3.m);
}

void init()
{
	// Create 3 vertices that make up a triangle that fits on the viewport 
	GLfloat vertices[] = {
						  // Triangle 1
						  -0.5f, 0.0f, 0.0f,
						   0.5f, 0.0f, 0.0f,
						   0.0f, 1.0f, 0.0f,
						   // Triangle 2
						  -1.0f, -1.0f, 0.0f,
						   0.0f, -1.0f, 0.0f,
						  -0.5f, 0.0f, 0.0f,
						  // Triangle 3
						  -0.0f, -1.0f, 0.0f,
						   1.0f, -1.0f, 0.0f,
						   0.5f,  0.0f, 0.0f,
	};

	// Create a color array that identfies the colors of each vertex (format R, G, B, A)
	GLfloat colors[] = {1.0f, 0.0f, 0.0f, 1.0f,
						1.0f, 0.0f, 0.0f, 1.0f,
						1.0f, 0.0f, 0.0f, 1.0f,
						0.0f, 0.0f, 1.0f, 1.0f,
						0.0f, 0.0f, 1.0f, 1.0f,
						0.0f, 0.0f, 1.0f, 1.0f,
						0.0f, 1.0f, 0.0f, 1.0f,
						0.0f, 1.0f, 0.0f, 1.0f,
						0.0f, 1.0f, 0.0f, 1.0f};
	GLint numVertices = 9;
	// Set up the shaders
	GLuint shaderProgramID = CompileShaders();
	// Put the vertices and colors into a vertex buffer object
	generateObjectBuffer(vertices, colors, numVertices);
	// Link the current buffer to the shader
	linkCurrentBuffertoShader(shaderProgramID, numVertices);
	// Link Matrix to Shader
	uniform_matid_1 = glGetUniformLocation(shaderProgramID, "matScTrRo_1");
	glUniformMatrix4fv(uniform_matid_1, 1, GL_FALSE, matSTR_1.m);
	uniform_matid_2 = glGetUniformLocation(shaderProgramID, "matScTrRo_2");
	glUniformMatrix4fv(uniform_matid_2, 1, GL_FALSE, matSTR_2.m);
	uniform_matid_3 = glGetUniformLocation(shaderProgramID, "matScTrRo_3");
	glUniformMatrix4fv(uniform_matid_3, 1, GL_FALSE, matSTR_3.m);

}

int main(int argc, char** argv){
	//matSTR.m[12] = 0.74;
	// Set up the window
	glutInit(&argc, argv);
	// glutInitDisplay and Window creation
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Hello Lab #2");
	// Keyboard Function Call
	glutKeyboardFunc(keyInputs);
	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(display);
	 // A call to glewInit() must be done after glut is initialized!
    GLenum res = glewInit();
	// Check for any errors
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }
	for (size_t i = 0; i < 127; i++) {
		keyArray[i] = false;
	}

	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
    return 0;
}

