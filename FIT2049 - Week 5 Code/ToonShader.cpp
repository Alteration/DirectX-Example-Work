#include "ToonShader.h"

ToonShader::ToonShader(SceneLighting* lighting, Camera* cam, Texture* rampTexture) : PhongShader(lighting, cam)
{
	m_rampTexture = rampTexture;
	m_rampSampler = NULL;
}

ToonShader::~ToonShader()
{

}

bool ToonShader::Initialise(ID3D11Device* device, LPCWSTR vertexFilename, LPCWSTR pixelFilename, LPCWSTR geometryFilename)
{
	if (!PhongShader::Initialise(device, vertexFilename, pixelFilename, NULL))
	{
		return false;
	}
	D3D11_SAMPLER_DESC rampSamplerDescription;
	//Now all we need to do is fill out our sampler description
	rampSamplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;		//This is the Filtering method used for the texture... 
	//...different values will give you different quality texture output
	rampSamplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//This defines what happens when we sample outside of the range [0...1]
	rampSamplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		//In our case we just want it to wrap around so that we always are sampling something
	rampSamplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	rampSamplerDescription.MipLODBias = 0.0f;							//The rest of these values are really just "Defaults"
	rampSamplerDescription.MaxAnisotropy = 1;							//If you want more info look up D3D11_SAMPLER_DESC on MSDN
	rampSamplerDescription.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	rampSamplerDescription.BorderColor[0] = 1;
	rampSamplerDescription.BorderColor[1] = 1;
	rampSamplerDescription.BorderColor[2] = 1;
	rampSamplerDescription.BorderColor[3] = 1;
	rampSamplerDescription.MinLOD = 0;
	rampSamplerDescription.MaxLOD = D3D11_FLOAT32_MAX;

	//and create the sampler!
	if (FAILED(device->CreateSamplerState(&rampSamplerDescription, &m_rampSampler)))
	{
		return false;
	}

	return true;
}

void ToonShader::Begin(ID3D11DeviceContext* context)
{
	PhongShader::Begin(context);

	context->PSGetSamplers(1, 1, &m_rampSampler);
}
bool ToonShader::SetTexture(ID3D11DeviceContext* context, ID3D11ShaderResourceView* textureView)
{
	if (!PhongShader::SetTexture(context, textureView))
	{
		return false;
	}
	ID3D11ShaderResourceView* rampView = m_rampTexture->GetShaderResourceView();
	context->PSSetShaderResources(1, 1, &rampView);

	return true;
}
void ToonShader::Release()
{
	PhongShader::Release();

	if (m_rampSampler)
	{
		m_rampSampler->Release();
		m_rampSampler = NULL;
	}
}