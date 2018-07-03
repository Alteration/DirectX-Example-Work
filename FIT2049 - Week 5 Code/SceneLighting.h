#ifndef SCENELIGHTING_H
#define SCENELIGHTING_H

#include "DirectXTK/SimpleMath.h"

using namespace DirectX::SimpleMath;

class SceneLighting
{
private:
	Vector3 m_lightDirection;
	float m_lightIntensity;
	Color m_ambientLight;

public:
	SceneLighting();
	SceneLighting(Vector3 lightDir, float lightIntensity, Color ambientLight);
	~SceneLighting();

	Vector3 GetLightDirection() { return m_lightDirection; }
	float GetLightIntensity() { return m_lightIntensity; }
	Color GetAmbientLight() { return m_ambientLight; }

	void SetAmbientLight(Color ambient) { m_ambientLight = ambient; }
	void SetLightDirection(Vector3 direction) { m_lightDirection = direction; }
};

#endif