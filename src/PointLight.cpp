#include "PointLight.h"

PointLight::PointLight()
	:
	name(PointLight::Name::UNDEFINED), 
	Light{},
	Position(),
	Attenuation(),
	Range(0.0f)
{
}

PointLight::~PointLight()
{
}

const Vect& PointLight::GetPosition() const
{
	return (this->Position);
}

void PointLight::SetPosition(const Vect newPos)
{
	this->Position = newPos;
}

const PointLight::Name PointLight::GetName() const
{
	return (this->name);
}

void PointLight::SetName(const PointLight::Name newName)
{
	this->name = newName;
}

const PointLight::PhongADS& PointLight::GetLight() const
{
	return this->Light;
}

void PointLight::SetLight(const Vect newAmb, const Vect newDiffuse, const Vect newSpec)
{
	this->Light.Ambient = newAmb;
	this->Light.Diffuse = newDiffuse;
	this->Light.Specular = newSpec;
}

const Vect& PointLight::GetAttenuation() const
{
	return this->Attenuation;
}

void PointLight::SetAttenuation(const Vect newAtten)
{
	this->Attenuation = newAtten;
}

const float& PointLight::GetRange() const
{
	return(this->Range);
}

void PointLight::SetRange(const float newRange)
{
	this->Range = newRange;
}

void PointLight::SetPointLightParameters(const Vect& pos, float r, const Vect& att, const Vect& amb, const Vect& dif, const Vect& sp)
{
	this->Light.Ambient = amb;
	this->Light.Diffuse = dif;
	this->Light.Specular = sp;
	this->Position = pos;
	this->Attenuation = att;
	this->Range = r;
}