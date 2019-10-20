#include <vector>
#include <string>
#include "maths_funcs.h"

bool loadOBJ(
	const char* path,
	std::vector < vec3 >& out_vertices,
	std::vector < vec2 >& out_uvs,
	std::vector < vec3 >& out_normals
);