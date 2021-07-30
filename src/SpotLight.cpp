#include "SpotLight.h"

SpotLight::SpotLight()
	:
	name(SpotLight::Name::UNDEFINED),
	Light{},
	Position(),
	Attenuation(),
	Direction(),
	SpotExp(0.0f),
	Range(0.0f)
{
}

SpotLight::~SpotLight()
{
}

const Vect& SpotLight::GetPosition() const
{
	return (this->Position);
}

void SpotLight::SetPosition(const Vect newPos)
{
	this->Position = newPos;
}

const SpotLight::Name SpotLight::GetName() const
{
	return (this->name);
}

void SpotLight::SetName(const SpotLight::Name newName)
{
	this->name = newName;
}

const SpotLight::PhongADS& SpotLight::GetLight() const
{
	return this->Light;
}

void SpotLight::SetLight(const Vect newAmb, const Vect newDiffuse, const Vect newSpec)
{
	this->Light.Ambient = newAmb;
	this->Light.Diffuse = newDiffuse;
	this->Light.Specular = newSpec;
}

const Vect& SpotLight::GetAttenuation() const
{
	return this->Attenuation;
}

void SpotLight::SetAttenuation(const Vect newAtten)
{
	this->Attenuation = newAtten;
}

const Vect& SpotLight::GetDirection() const
{
	return (this->Direction);
}

void SpotLight::SetDirection(const Vect newDir)
{
	this->Direction = newDir;
}

const float& SpotLight::GetRange() const
{
	return(this->Range);
}

void SpotLight::SetRange(const float newRange)
{
	this->Range = newRange;
}

const float& SpotLight::GetExposure() const
{
	return (this->SpotExp);
}

void SpotLight::SetExposure(const float newExp)
{
	this->SpotExp = newExp;
}

void SpotLight::SetSpotLightParameters(const Vect& pos, float r, const Vect& att, const Vect& dir, float spotExp, const Vect& amb, const Vect& dif, const Vect& sp)
{
	this->Light.Ambient = amb;
	this->Light.Diffuse = dif;
	this->Light.Specular = sp;
	this->Position = pos;
	this->Direction = dir;
	this->Attenuation = att;
	this->Range = r;
	this->SpotExp = spotExp;
}