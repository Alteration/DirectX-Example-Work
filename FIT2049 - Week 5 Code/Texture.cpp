/*	FIT2049 - Example Code
*	Texture.cpp
*	Created by Elliott Wilson - 2015 - Monash Univeristy
*	Implementation of Texture.h
*/

#include "Texture.h"
#include "DirectXTK/WICTextureLoader.h"

using namespace DirectX;

Texture::Texture()
{
	m_texture = NULL;
	m_textureView = NULL;
}

Texture::~Texture()
{

}

bool Texture::Initialise(Direct3D* direct3D, LPCWSTR filename)
{
	//This method uses the CreateWICTextureFromFile function. This function comes from the DirectXToolKit library

	HRESULT result = CreateWICTextureFromFile(direct3D->GetDevice(), filename, &m_texture, &m_textureView);

	if (FAILED(result))
	{
		return false;
	}
	m_filename = filename;
	return true;
}

void Texture::Release()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = NULL;
	}

	if (m_textureView)
	{
		m_textureView->Release();
		m_textureView = NULL;
	}
}