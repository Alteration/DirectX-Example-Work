#include "PhongShader.h"

PhongShader::PhongShader(SceneLighting* lightingSetup, Camera* cam) : Shader()
{
	m_lightSetup = lightingSetup;
	m_camera = cam;
	m_shineFactor = 8.0f;
}

PhongShader::~PhongShader()
{

}

bool PhongShader::Initialise(ID3D11Device* device, LPCWSTR vertexFilename, LPCWSTR pixelFilename, LPCWSTR geometryFilename)
{
	D3D11_SAMPLER_DESC textureSamplerDescription;	//When we create a sampler we need a Description struct to describe how we want to create the sampler

	if (!Shader::Initialise(device, vertexFilename, pixelFilename, NULL))		//We'll use the parent method to create most of the shader
	{
		return false;
	}

	//Now all we need to do is fill out our sampler description
	textureSamplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;		//This is the Filtering method used for the texture... 
	//...different values will give you different quality texture output
	textureSamplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//This defines what happens when we sample outside of the range [0...1]
	textureSamplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		//In our case we just want it to wrap around so that we always are sampling something
	textureSamplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	textureSamplerDescription.MipLODBias = 0.0f;							//The rest of these values are really just "Defaults"
	textureSamplerDescription.MaxAnisotropy = 1;							//If you want more info look up D3D11_SAMPLER_DESC on MSDN
	textureSamplerDescription.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	textureSamplerDescription.BorderColor[0] = 1;
	textureSamplerDescription.BorderColor[1] = 1;
	textureSamplerDescription.BorderColor[2] = 1;
	textureSamplerDescription.BorderColor[3] = 1;
	textureSamplerDescription.MinLOD = 0;
	textureSamplerDescription.MaxLOD = D3D11_FLOAT32_MAX;

	//and create the sampler!
	if (FAILED(device->CreateSamplerState(&textureSamplerDescription, &m_textureSampler)))
	{
		return false;
	}

	D3D11_BUFFER_DESC lightBufferDescription;
	lightBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDescription.ByteWidth = sizeof(LightBuffer);
	lightBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDescription.MiscFlags = 0;
	lightBufferDescription.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&lightBufferDescription, NULL, &m_lightBuffer)))
	{
		return false;
	}

	D3D11_BUFFER_DESC cameraBufferDescription;
	cameraBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDescription.ByteWidth = sizeof(CameraBuffer);
	cameraBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDescription.MiscFlags = 0;
	cameraBufferDescription.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&cameraBufferDescription, NULL, &m_cameraBuffer)))
	{
		return false;
	}

	return true;
}

void PhongShader::Begin(ID3D11DeviceContext* context)
{
	Shader::Begin(context);		//Let the parent class do most of the work!
	context->PSSetSamplers(0, 1, &m_textureSampler);
	context->PSSetConstantBuffers(0, 1, &m_lightBuffer);

	context->VSSetConstantBuffers(1, 1, &m_cameraBuffer);
}

bool PhongShader::SetConstants(ID3D11DeviceContext* context, Matrix world, Matrix view, Matrix projection)
{
	if (!Shader::SetConstants(context, world, view, projection))
	{
		return false;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	LightBuffer* inputData;

	if (FAILED(context->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	inputData = (LightBuffer*)mappedResource.pData;

	inputData->lightDir = (m_lightSetup->GetLightDirection());
	inputData->lightIntensity = m_lightSetup->GetLightIntensity();
	inputData->ambientLight = m_lightSetup->GetAmbientLight();
	inputData->shine = m_shineFactor;

	context->Unmap(m_lightBuffer, 0);

	CameraBuffer* inputCam;

	if (FAILED(context->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	inputCam = (CameraBuffer*)mappedResource.pData;
	inputCam->camPos = m_camera->GetPosition();

	context->Unmap(m_cameraBuffer, 0);

	return true;
}

bool PhongShader::SetTexture(ID3D11DeviceContext* context, ID3D11ShaderResourceView* textureView)
{
	//This shader does have a texture so it does something in this method!
	context->PSSetShaderResources(0, 1, &textureView);
	return true;
}

void PhongShader::Release()
{
	Shader::Release();
	if (m_textureSampler)
	{
		m_textureSampler->Release();
		m_textureSampler = NULL;
	}
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = NULL; 
	}
	if (m_cameraBuffer)
	{
		m_cameraBuffer->Release();
		m_cameraBuffer = NULL;
	}
}