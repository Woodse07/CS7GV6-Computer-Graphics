#include <blender_loader.h>

blenderObj::blenderObj(std::string filename) {
	std::ifstream ifs(filename.c_str(), std::ifstream::in);
	std::string line, key;
	while (ifs.good() && !ifs.eof() && std::getline(ifs, line)) {
		key = "";
		std::stringstream stringstream(line);
		stringstream >> key >> std::ws;

		if (key == "v") { // vertex
			vertex v; float x;
			while (!stringstream.eof()) {
				stringstream >> x >> std::ws;
				v.v.push_back(x);
			}
			vertices.push_back(v);
		}
		else if (key == "vp") { // parameter
			vertex v; float x;
			while (!stringstream.eof()) {
				stringstream >> x >> std::ws;
				v.v.push_back(x);
			}
			parameters.push_back(v);
		}
		else if (key == "vt") { // texture coordinate
			vertex v; float x;
			while (!stringstream.eof()) {
				stringstream >> x >> std::ws;
				v.v.push_back(x);
			}
			texcoords.push_back(v);
		}
		else if (key == "vn") { // normal
			vertex v; float x;
			while (!stringstream.eof()) {
				stringstream >> x >> std::ws;
				v.v.push_back(x);
			}
			v.normalize();
			normals.push_back(v);
		}
		else if (key == "f") { // face
			face f; int v, t, n;
			while (!stringstream.eof()) {
				stringstream >> v >> std::ws;
				f.vertex.push_back(v - 1);
				if (stringstream.peek() == '/') {
					stringstream.get();
					if (stringstream.peek() == '/') {
						stringstream.get();
						stringstream >> n >> std::ws;
						f.normal.push_back(n - 1);
					}
					else {
						stringstream >> t >> std::ws;
						f.texture.push_back(t - 1);
						if (stringstream.peek() == '/') {
							stringstream.get();
							stringstream >> n >> std::ws;
							f.normal.push_back(n - 1);
						}
					}
				}
			}
			faces.push_back(f);
		}
		else {

		}
	}
	ifs.close();
	std::cout << "               Name: " << filename << std::endl;
	std::cout << "           Vertices: " << vertices.size() << std::endl;
	std::cout << "         Parameters: " << parameters.size() << std::endl;
	std::cout << "Texture Coordinates: " << texcoords.size() << std::endl;
	std::cout << "            Normals: " << normals.size() << std::endl;
	std::cout << "              Faces: " << faces.size() << std::endl << std::endl;

	for (int faces_index = 0; faces_index < faces.size(); faces_index++) {
		for (int vertex_index = 0; vertex_index < faces[faces_index].vertex.size(); vertex_index++) {
			float_vertices.push_back(vertices[faces[faces_index].vertex[vertex_index]].v[0]);
			float_vertices.push_back(vertices[faces[faces_index].vertex[vertex_index]].v[1]);
			float_vertices.push_back(vertices[faces[faces_index].vertex[vertex_index]].v[2]);
		}
		for (int texture_index = 0; texture_index < faces[texture_index].texture.size(); texture_index++) {
			float_tex.push_back(texcoords[faces[faces_index].texture[texture_index]].v[0]);
			float_tex.push_back(texcoords[faces[faces_index].texture[texture_index]].v[1]);
		}
		for (int normal_index = 0; normal_index < faces[normal_index].normal.size(); normal_index++) {
			float_normals.push_back(normals[faces[faces_index].normal[normal_index]].v[0]);
			float_normals.push_back(normals[faces[faces_index].normal[normal_index]].v[1]);
			float_normals.push_back(normals[faces[faces_index].normal[normal_index]].v[2]);
		}
	}
	numvertices = float_vertices.size() / 3;
	std::cout << "    Face Vertices: " << float_vertices.size() << std::endl;
	std::cout << "    Face Textures: " << float_normals.size() << std::endl;
	std::cout << "     Face Normals:: " << float_normals.size() << std::endl;
	vertices.clear();
	texcoords.clear();
	normals.clear();
	faces.clear();
}
