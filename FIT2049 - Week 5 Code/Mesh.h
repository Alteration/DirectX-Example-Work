/*	FIT2049 - Example Code
*	Mesh.h
*	Created by Elliott Wilson - 2015 - Monash Univeristy
*	This class represents an indexed Mesh of 3D Vertices.
*	It is capable of loaded triangulated OBJ files as well as creating simple triangles
*	It can also render the mesh using a provided Shader object
*/

#ifndef MESH_H
#define MESH_H

#include <d3d11.h>
#include "Direct3D.h"
#include "DirectXTK/SimpleMath.h"
#include "Camera.h"
#include "Texture.h"

using namespace DirectX::SimpleMath;

class Mesh
{
private:
	struct Vertex			//This struct represents a single vertex of our mesh
	{
		Vector3 position;	//It is made up of a position
		Color colour;		//A colour
		Vector3 normal;		//A normal, which defines which way the face is "facing", these are important for lighting calculations
		Vector2 texCoord;	//and a texture coordinate, these are used to determine which part of a texture belongs to this vertex
	};
	int m_vertexCount;		//We store a count of all of the vertices
	int m_indexCount;		//As well as a count of all of the indices
	ID3D11Buffer* m_vertexBuffer;	//We use Direct3D Buffers to store the verts...
	ID3D11Buffer* m_indexBuffer;	//... and the indices in graphics memory

	Shader* m_meshShader;	//When a mesh in rendered it is rendered according to a certain shader, we store a pointer to that shader here
	Texture* m_meshTexture;	//The texture that is referenced by the uvs of the mesh, this will be passed into the pixel shader

	bool InitialiseBuffers(Direct3D* direct3D, Vertex* vertexData, unsigned long* indexData);	//This method takes vertex and index data and creates the Direct3D buffers

public:
	Mesh();		//Constructor
	~Mesh();	//Destructor

	bool Initialise(Direct3D* direct3D, Shader* meshShader);	//This initialise method creates a simple double sided triangle
	bool Initialise(Direct3D* direct3D, const char* filename, Color defaultColor, Texture* texture, Shader* meshShader);	//This initialise method attempts to load the vert 
																															//and index data from an OBJ file. It also allows 
																															//you to specify the overall colour of the mesh and
																															//the texture to use (the texture can be NULL)
	void Release();		///Performs cleanup
	void Render(Direct3D* direct3D, Matrix world, Camera* cam);		//Renders the mesh data according to the mesh shader and the world matrix supplied

	//Accessors
	int GetVertexCount() { return m_vertexCount; }
	int GetIndexCount() { return m_indexCount; }
};

#endif