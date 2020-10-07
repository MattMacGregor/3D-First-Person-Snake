#pragma once
#include <glm\glm.hpp>
#include <string>
#include <vector>
#include <Shader.h>
#include <sstream>
#include <assimp/types.h>
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};
struct Texture
{
	unsigned int id;
	std::string type;
	aiString path;
};
class Mesh
{
public:
	/* Mesh Data */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	float shininess;
	/* Functions */
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, float shininess = 1.0f);
	void draw(Shader shader);
private:
	/* Render Data */
	unsigned int VAO, VBO, EBO;
	/* Functions */
	void setupMesh();
};
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, float shininess)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->shininess = shininess;

	setupMesh();
}
void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}
void Mesh::draw(Shader shader)
{
	unsigned int diffuseNr = 1, specularNr = 1;
	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::stringstream ss;
		std::string name = textures[i].type;
		std::string number;
		if (name == "texture_diffuse")
			ss << diffuseNr++;
		else if (name == "texture_specular")
			ss << specularNr++;
		number == ss.str();
		
		shader.setFloat(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);
	shader.setFloat("material.shininess", shininess);
	//draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
