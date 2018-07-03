/*	FIT2049 - Example Code
*	Texture.h
*	Created by Elliott Wilson - 2015 - Monash Univeristy
*	This class stores the required information for a single texture.
*	It is also used to load textures from a number of different file formats
*	It uses the functions from the DirectX Tool Kit to achieve this.
*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <d3d11.h>
#include "Direct3D.h"

class Texture
{
private:
	LPCWSTR m_filename;							//The filename of the texture that was loaded
	ID3D11Resource* m_texture;					//The texture resource, can be used for procedurally modifying the texture
	ID3D11ShaderResourceView* m_textureView;	//The texture view, this is passed into our Shaders and the Sprite Batch
	
public:
	Texture();	//Constructor
	~Texture();	//Destructor

	bool Initialise(Direct3D* direct3D, LPCWSTR filename);	//Initialises a texture by load it from a file
	void Release();		//Clean up

	//Accessors
	ID3D11Resource* GetTexture() { return m_texture; }
	ID3D11ShaderResourceView* GetShaderResourceView() { return m_textureView; }
};

#endif