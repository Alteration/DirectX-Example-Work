#ifndef TOONSHADER_H
#define TOONSHADER_H

#include "PhongShader.h"
#include "Texture.h"

class ToonShader : public PhongShader
{
private:
	Texture* m_rampTexture;
	ID3D11SamplerState* m_rampSampler;

public:
	ToonShader(SceneLighting* lighting, Camera* cam, Texture* rampTexture);
	~ToonShader();

	void Begin(ID3D11DeviceContext* context);
	bool Initialise(ID3D11Device* device, LPCWSTR vertexFilename, LPCWSTR pixelFilename, LPCWSTR geometryFilename);
	bool SetTexture(ID3D11DeviceContext* context, ID3D11ShaderResourceView* textureView);
	void Release();
};

#endif