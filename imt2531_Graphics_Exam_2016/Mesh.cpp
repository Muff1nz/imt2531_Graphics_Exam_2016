#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "./libraries/tiny_obj_loader.h"

Mesh::Mesh(){
	VAO = IBO = drawSize = 0;
}

Mesh::Mesh(std::string modelPath, bool texture, glm::vec2 shapeRange) {
	VAO = IBO = drawSize = 0;
	createBufferObjects(loadModelData(modelPath, texture, shapeRange), texture);
}

Mesh::~Mesh(){
}

void Mesh::draw() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, drawSize);
}

/**
 * Creates and stores the needed VAO, VBO(s) and IBO for rendering the model
 * @param model A struct containing the needed data loaded from file.
 */
void Mesh::createBufferObjects(const ModelData& model, bool texture){
	GLuint VBO; //Im not interested in storing/using this later

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);	// Vertex Positions
	glEnableVertexAttribArray(1);	// Vertex Normals
	glEnableVertexAttribArray(2);	// Ambient
	glEnableVertexAttribArray(3);	// Diffuse
	glEnableVertexAttribArray(4);	// Specular
	glEnableVertexAttribArray(5);	// Specular exponent
	glEnableVertexAttribArray(6);	// Texture uv's

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, model.vertexPosition.size() * sizeof(GLfloat), &model.vertexPosition[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, model.vertexNormal.size() * sizeof(GLfloat), &model.vertexNormal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, model.ambient.size() * sizeof(GLfloat), &model.ambient[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, model.diffuse.size() * sizeof(GLfloat), &model.diffuse[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, model.specular.size() * sizeof(GLfloat), &model.specular[0], GL_STATIC_DRAW);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, model.specularExponent.size() * sizeof(GLfloat), &model.specularExponent[0], GL_STATIC_DRAW);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

	if (texture) { //Textures are optional
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, model.vertexUV.size() * sizeof(GLfloat), &model.vertexUV[0], GL_STATIC_DRAW);
		glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}
	

	drawSize = model.numberOfVertexes;

}

/**	
*	this function attempts to center and raise vertecies
*	Used when extracting models from model packs, to remove
*	the models offset from origin
* @param verts	vertices to center and raise
*/
void Mesh::centerAndRaise(std::vector<float> &verts) {
	glm::vec3 averageVertex(0); //Average vertex position for model
	float modelBase = verts[1]; //Lowest vertex, so model can be raised above ground
	for (int i = 0; i < verts.size(); i++) { //Compute average vertex and model base
		if (!i % 3) //X coordinate
			averageVertex.x += verts[i];

		if (!((i - 1) % 3)) { //Y coordinate
			if (modelBase > verts[i])
				modelBase = verts[i];
		}

		if (!((i - 2) % 3))  //Z coordinate
			averageVertex.z += verts[i];
	}

	averageVertex /= (verts.size() / 3);

	for (int i = 0; i < verts.size(); i++) { //Apply the offsets
		if (!i % 3) //X coordinate
			verts[i] -= averageVertex.x;

		if (!((i - 1) % 3)) //Y coordinate
			verts[i] -= modelBase;

		if (!((i - 2) % 3))  //Z coordinate
			verts[i] -= averageVertex.z;
	}
}

/**
 * Loads model data from obj and mtl files
 * @param  modelPath	path and name for the .obj file. F.eks: "./resources/models/car.obj"
 * @param  texture		if the model has texture coordinates or not
 * @param shapeRange	shape to start with, and shape to end with, only use if you have intimate knowlage of the .obj file
 * @return             A ModelData struct. It is used to pass the information to the createBufferObject function.
 */
ModelData Mesh::loadModelData(std::string modelPath, bool texture, glm::vec2 shapeRange)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	ModelData model;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, modelPath.c_str(), 
			modelPath.substr(0, modelPath.find_last_of('/') + 1).c_str())) {
		throw std::runtime_error(err);
	}

	if(!err.empty()) {
		printf("%s\n", err.c_str());
	}

	if (!shapeRange.y) shapeRange.y = shapes.size();

	// Loop over shapes
	printf("%d shapes in %s\n", shapes.size(), modelPath.c_str());
	for (size_t s = shapeRange.x; s < shapeRange.y; s++) {
		// Loop over faces(polygons)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				//This currently loads values multiple times
				///	I see now (03.12) that i forgot to optimize this with IBOs, and i don't
				///	want to mess with core code this late in the exam, so i leaving it as is
				for (int i = 0; i < 3; i++) {
					model.vertexPosition.push_back(attrib.vertices[model.vertexOrder*idx.vertex_index + i]);
					model.vertexNormal.push_back(attrib.normals[model.normalOrder*idx.normal_index + i]);	

					model.ambient.push_back(materials[shapes[s].mesh.material_ids[f]].ambient[i]);
					model.diffuse.push_back(materials[shapes[s].mesh.material_ids[f]].diffuse[i]);
					model.specular.push_back(materials[shapes[s].mesh.material_ids[f]].specular[i]);					
				}
				model.specularExponent.push_back(materials[shapes[s].mesh.material_ids[f]].shininess);

				//texture uvs
				if (texture) {
					model.vertexUV.push_back(attrib.texcoords[2 * idx.texcoord_index]);
					model.vertexUV.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
				}
			}
			index_offset += fv;
		}		
	}

	if (shapeRange != glm::vec2(0, shapes.size())) //Are we loading a subpart of the .obj?
		centerAndRaise(model.vertexPosition);	

	model.numberOfVertexes = model.vertexPosition.size();
	model.numberOfIndices = model.indices.size();
	return model;
}
