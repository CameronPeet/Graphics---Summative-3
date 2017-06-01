#pragma once

#include "glew/glew.h"
#include "freeglut/freeglut.h"
#include "soil/SOIL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <vector>

/*
Model Type
*/

enum ModelType {

	kTriangle = 0,
	kQuad = 1,
	kCube = 2,
	kSphere = 3

};


/*
Vertex Format
*/

struct Position {

	float x, y, z;

	Position(float _x, float _y, float _z) {

		x = _x; y = _y; z = _z;
	}

	Position() {}
};

struct TexCoord {

	float u, v;

	TexCoord(float _u, float _v) {
		u = _u; v = _v;
	}

	TexCoord() {}

};

struct Normals {

	float x, y, z;

	Normals(float _x, float _y, float _z) {

		x = _x; y = _y; z = _z;
	}

	Normals() {}
};

struct VertexFormat {

	Position pos;
	TexCoord texCoord;
	Normals normal;

	VertexFormat(Position _pos, TexCoord _texCoord, Normals _normal) {

		pos = _pos;
		texCoord = _texCoord;
		normal = _normal;

	}

	VertexFormat() {
	}
};

struct IndexFormat {

	int x; int y; int z;

	IndexFormat(int _x, int _y, int _z) {

		x = _x; y = _y; z = _z;
	}
};


class Utils {

public:

	const static int WIDTH = 1280;
	const static int HEIGHT = 720;

	//const static int WIDTH = 800;
	//const static int HEIGHT = 600;

	//const static int WIDTH = 2736;
	//const static int HEIGHT = 1824;

	static void setTriData(std::vector<VertexFormat>& vertices, std::vector<GLuint>&indices);
	static void setQuadData(std::vector<VertexFormat>& vertices, std::vector<GLuint>&indices);
	static void setCubeData(std::vector<VertexFormat>& vertices, std::vector<GLuint>&indices);
	static void setSphereData(std::vector<VertexFormat>& vertices, std::vector<GLuint>&indices);
};







////
//// Bachelor of Software Engineering
//// Media Design School
//// Auckland
//// New Zealand
////
//// File Name	:	Utils.h
//// Description	:	Utils declaration file.
//// Author		:	Charmaine Lim
////
//
//#ifndef UTILS_H
//#define UTILS_H
//
//#include <vector>
//
//#include "glm/glm.hpp"
//#include "glew/glew.h"
//
//using namespace std;
//using namespace glm;
//
//// a ModelType enum to specify model type to be created
//enum ModelType
//{
//	kTriangle,
//	kQuad,
//	kCube,
//	pyramid,
//	pentagon,
//	kSphere
//};
//
////struct SPosition
////{
////	vec3 position;
////};
////
////struct STexCoord
////{
////	vec2 texcoord;
////};
////
////struct SVertexFormat
////{
////	SPosition position;
////	vec3 colour;
////	STexCoord texCoord;
////};
//
//struct Position
//{
//	float x, y, z;
//
//	Position(float _x, float _y, float _z)
//	{
//		x = _x;
//		y = _y;
//		z = _z;
//	}
//
//	Position() {};
//};
//
//struct TexCoord
//{
//	float u, v;
//
//	TexCoord(float _x, float _y)
//	{
//		u = _x;
//		v = _y;
//	}
//
//	TexCoord() {};
//};
//
//struct Normals
//{
//	float x, y, z;
//
//	Normals(float _x, float _y, float _z)
//	{
//		x = _x;
//		y = _y;
//		z = _z;
//	}
//
//	Normals() {};
//};
//
//struct VertexFormat
//{
//	Position pos;
//	TexCoord texCoord;
//	Normals normal;
//
//	VertexFormat(Position _pos, TexCoord _texCoord, Normals _normal)
//	{
//		pos = _pos;
//		texCoord = _texCoord;
//		normal = _normal;
//	}
//
//	VertexFormat() {};
//};
//
//struct IndexFormat
//{
//	int x, y, z;
//
//	IndexFormat(int _x, int _y, int _z)
//	{
//		x = _x;
//		y = _y;
//		z = _z;
//	}
//
//	IndexFormat() {};
//};
//
////struct SIndexFormat
////{
////	vector<GLuint> vec_indices;
////};
//
//class Utils
//{
//
//public:
//
//	const static int WIDTH = 1280;
//	const static int HEIGHT = 720;
//
//	static void setTriData(std::vector<VertexFormat>& vertices, std::vector<GLuint>& indices);
//	static void setQuadData(std::vector<VertexFormat>& vertices, std::vector<GLuint>& indices);
//	static void setCubeData(std::vector<VertexFormat>& vertices, std::vector<GLuint>& indices);
//	static void setSphereData(std::vector<VertexFormat>& vertices, std::vector<GLuint>& indices);
//
//};
//
//
////struct SVertices
////{
////	vector<SVertexFormat> vec_Vertices;
////	SIndexFormat vec_Indices;
////};
////
////static vector<SVertexFormat> StoreVertices(GLfloat* _vertices, int _iSize)
////{
////	vector<SVertexFormat> vec_Vertices;
////
////	for (int i = 0; i <= _iSize - 8; i += 8)
////	{
////		SPosition vertexCoord;
////		vertexCoord.position = vec3(_vertices[i], _vertices[i + 1], _vertices[i + 2]);
////
////		vec3 colourCoord = vec3(_vertices[i + 3], _vertices[i + 4], _vertices[i + 5]);
////
////		STexCoord textureCoord;
////		textureCoord.texcoord = vec2(_vertices[i + 6], _vertices[i + 7]);
////
////		SVertexFormat tempVertex;
////		tempVertex.position = vertexCoord;
////		tempVertex.colour = colourCoord;
////		tempVertex.texCoord = textureCoord;
////		vec_Vertices.push_back(tempVertex);
////	}
////
////	return vec_Vertices;
////}
////
////static SIndexFormat StoreIndices(GLuint* _indices, int _iSize)
////{
////	SIndexFormat vec_Indices;
////
////	for (int i = 0; i < _iSize; i++)
////	{
////		vec_Indices.vec_indices.push_back(_indices[i]);
////	}
////
////	return vec_Indices;
////}
////
////static SVertices GroupTogether(vector<SVertexFormat> _vec_Vertices, SIndexFormat _vec_Indices)
////{
////	SVertices verticesInfo;
////	verticesInfo.vec_Vertices = _vec_Vertices;
////	verticesInfo.vec_Indices = _vec_Indices;
////
////	return verticesInfo;
////}
////
////static SVertices Process(GLfloat* _vertices, int iSizeVert, GLuint* _indices, int iSizeInd)
////{
////	vector<SVertexFormat> vec_Vertices = StoreVertices(_vertices, iSizeVert);
////
////	SIndexFormat vec_Indices = StoreIndices(_indices, iSizeInd);
////
////	return GroupTogether(vec_Vertices, vec_Indices);
////}
////
////static SVertexFormat CombineVertex(SPosition _position, vec3 _colour, STexCoord _texture)
////{
////	SVertexFormat tempVertex;
////	tempVertex.position = _position;
////	tempVertex.colour = _colour;
////	tempVertex.texCoord = _texture;
////
////	return tempVertex;
////}
////
//////Static functions to set vertices for Triangle, Quad, Cube, etc.
////static SVertices setTriangleVertices()
////{
////	vector<SVertexFormat> vec_Vertices;
////
////	SPosition BLposition, BRposition, topPosition;
////	BLposition.position = vec3(-0.5f, 0.433f, 0.0f);
////	BRposition.position = vec3(0.5f, 0.433f, 0.0f);
////	topPosition.position = vec3(0.0f, -0.433f, 0.0f);
////
////	vec3 BLcolour = vec3(1.0f, 0.0f, 0.0f);
////	vec3 BRcolour = vec3(0.0f, 1.0f, 0.0f);
////	vec3 topcolour = vec3(0.0f, 0.0f, 1.0f);
////
////	STexCoord BLtexture, BRtexture, topTexture;
////	BLtexture.texcoord = vec2(1.0f, 1.0f);
////	BRtexture.texcoord = vec2(0.0f, 1.0f);
////	topTexture.texcoord = vec2(0.5f, 0.0f);
////
////	SIndexFormat vec_Indices;
////	vec_Indices.vec_indices.push_back(0);
////	vec_Indices.vec_indices.push_back(1);
////	vec_Indices.vec_indices.push_back(2);
////
////	vec_Vertices.push_back(CombineVertex(BLposition, BLcolour, BLtexture));
////
////	vec_Vertices.push_back(CombineVertex(BRposition, BRcolour, BRtexture));
////
////	vec_Vertices.push_back(CombineVertex(topPosition, topcolour, topTexture));
////
////	return GroupTogether(vec_Vertices, vec_Indices);
////}
////
////static SVertices setQuadVertices()
////{
////	vector<SVertexFormat> vec_Vertices;
////
////	SPosition TRposition, BRposition, BLposition, TLposition;
////	TLposition.position = vec3(-0.5f, 0.5f, 0.0f);
////	TRposition.position = vec3(0.5f, 0.5f, 0.0f);
////	BRposition.position = vec3(0.5f, -0.5f, 0.0f);
////	BLposition.position = vec3(-0.5f, -0.5f, 0.0f);
////
////	vec3 TLcolour = vec3(1.0f, 0.0f, 1.0f);
////	vec3 TRcolour = vec3(0.0f, 1.0f, 1.0f);
////	vec3 BRcolour = vec3(1.0f, 1.0f, 0.0f);
////	vec3 BLcolour = vec3(0.0f, 0.0f, 0.0f);
////
////	STexCoord TRtexture, BRtexture, BLtexture, TLtexture;
////	TLtexture.texcoord = vec2(0.0f, 1.0f);
////	TRtexture.texcoord = vec2(1.0f, 1.0f);
////	BRtexture.texcoord = vec2(1.0f, 0.0f);
////	BLtexture.texcoord = vec2(0.0f, 0.0f);
////
////	SIndexFormat vec_Indices;
////	vec_Indices.vec_indices.push_back(0);
////	vec_Indices.vec_indices.push_back(1);
////	vec_Indices.vec_indices.push_back(2);
////
////	vec_Indices.vec_indices.push_back(0);
////	vec_Indices.vec_indices.push_back(2);
////	vec_Indices.vec_indices.push_back(3);
////
////	vec_Vertices.push_back(CombineVertex(TLposition, TLcolour, TLtexture));
////
////	vec_Vertices.push_back(CombineVertex(TRposition, TRcolour, TRtexture));
////
////	vec_Vertices.push_back(CombineVertex(BRposition, BRcolour, BRtexture));
////
////	vec_Vertices.push_back(CombineVertex(BLposition, BLcolour, BLtexture));
////
////	return GroupTogether(vec_Vertices, vec_Indices);
////}
////
////static SVertices setCubeVertices()
////{
////	GLfloat vertices[] = {
////
////		//position		//color					//texture coord
////		// front face
////		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
////		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, -0.0f, 0.0f, 0.0f,
////		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, -0.0f, 1.0f, 0.0f,
////		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
////
////		// Fill in the back face vertex data.
////		-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
////		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
////		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
////		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
////
////		// Fill in the top face vertex data.
////		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
////		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
////		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
////		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
////
////		// Fill in the bottom face vertex data.
////		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
////		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
////		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
////		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
////
////		// Fill in the left face vertex data.
////		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
////		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
////		-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
////		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
////
////		// Fill in the right face vertex data.
////		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
////		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
////		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
////		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
////	};
////
////	GLuint indices[] = {
////		// front
////		0, 1, 2,
////		0, 2, 3,
////		// top
////		4, 5, 6,
////		4, 6, 7,
////		//0, 1, 8,
////		//1, 8, 9,
////		// back
////		8, 9, 10,
////		8, 10, 11,
////		// bottom
////		12, 13, 14,
////		12, 14, 15,
////		// left
////		16, 17, 18,
////		16, 18, 19,
////		// right
////		20, 21, 22,
////		20, 22, 23,
////	};
////
////	int iSizeVert = sizeof(vertices) / sizeof(*vertices);
////
////	int iSizeInd = sizeof(indices) / sizeof(*indices);
////
////	return Process(vertices, iSizeVert, indices, iSizeInd);
////}
////
////static SVertices setPyramidVertices()
////{
////	GLfloat vertices[] = {
////
////		//position		//color					//texture coord
////		// front face
////		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
////		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.0f, 1.0f, 0.0f,
////		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, -0.0f, 0.5f, 0.0f,
////		0.0f, -0.5f, 0.5f, 1.0f, 0.0f, -0.0f, 0.0f, 1.0f,
////
////		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
////		0.0f, -0.5f, 0.5f, 1.0f, 0.0f, -0.0f, 1.0f, 0.0f,
////		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, -0.0f, 0.0f, 1.0f,
////	};
////
////
////	GLuint indices[] = {
////		0, 1, 2,
////		1, 3, 2,
////		4, 5, 6,
////		0, 1, 3,
////	};
////
////	int iSizeVert = sizeof(vertices) / sizeof(*vertices);
////
////	int iSizeInd = sizeof(indices) / sizeof(*indices);
////
////	return Process(vertices, iSizeVert, indices, iSizeInd);
////}
////
////static SVertices setPentagonVertices()
////{
////	GLfloat vertices[] = {
////		//position		//color					//texture coord
////		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
////		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, -0.0f, 0.0f, 0.0f,
////		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, -0.0f, 1.0f, 0.0f,
////		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
////		0.0f, 1.0f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
////	};
////
////	GLuint indices[] = {
////
////		0, 1, 2,
////		0, 2, 3,
////		1, 4, 2
////	};
////
////	int iSizeVert = sizeof(vertices) / sizeof(*vertices);
////
////	int iSizeInd = sizeof(indices) / sizeof(*indices);
////
////	return Process(vertices, iSizeVert, indices, iSizeInd);
////}
//
//
//
//
//
//
//#endif