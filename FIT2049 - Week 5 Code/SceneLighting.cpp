#include "SceneLighting.h"

SceneLighting::SceneLighting()
{
	m_lightDirection = Vector3(0, 0.5, -0.5);
	m_lightIntensity = 1.0f;
	m_ambientLight = Color(0.0f, 0.0f, 0.0f, 1.0f);
}

SceneLighting::~SceneLighting()
{

}

SceneLighting::SceneLighting(Vector3 lightDir, float lightIntensity, Color ambientLight)
{
	m_lightDirection = lightDir;
	m_lightIntensity = lightIntensity;
	m_ambientLight = ambientLight;
}