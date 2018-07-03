#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include "Shader.h"
#include "SceneLighting.h"
#include "Camera.h"

class PhongShader : public Shader
{
protected: 
	struct LightBuffer
	{
		Vector3 lightDir;
		float lightIntensity;
		Color ambientLight;
		float shine;
		Vector3 padding;
	};
	struct CameraBuffer
	{
		Vector3 camPos;
		float padding;
	};

	SceneLighting* m_lightSetup;
	Camera* m_camera;
	float m_shineFactor;
	ID3D11SamplerState* m_textureSampler;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_cameraBuffer;

public:
	PhongShader(SceneLighting* lightSetup, Camera* cam);	//Constructor
	virtual ~PhongShader();	//Destructor

	virtual void Begin(ID3D11DeviceContext* context);
	virtual bool Initialise(ID3D11Device* device, LPCWSTR vertexFilename, LPCWSTR pixelFilename, LPCWSTR geometryFilename);
	virtual bool SetConstants(ID3D11DeviceContext* context, Matrix world, Matrix view, Matrix projection);
	virtual bool SetTexture(ID3D11DeviceContext* context, ID3D11ShaderResourceView* textureView);
	virtual void Release();

	void SetShineFactor(float shine) { m_shineFactor = shine; }
};

#endif