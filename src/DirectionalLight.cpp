#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	:
	name(DirectionalLight::Name::UNDEFINED),
	Light{},
	Direction()
{
}

DirectionalLight::~DirectionalLight()
{
}

const DirectionalLight::Name& DirectionalLight::GetName() const
{
	return (this->name);
}

void DirectionalLight::SetName(const DirectionalLight::Name newName)
{
	this->name = newName;
}

const DirectionalLight::PhongADS& DirectionalLight::GetLight() const
{
	return this->Light;
}

void DirectionalLight::SetLight(const Vect newAmb, const Vect newDiffuse, const Vect newSpec)
{
	this->Light.Ambient = newAmb;
	this->Light.Diffuse = newDiffuse;
	this->Light.Specular = newSpec;
}

const Vect& DirectionalLight::GetDirection() const
{
	return this->Direction;
}

void DirectionalLight::SetDirection(const Vect newDir)
{
	this->Direction = newDir;
}

void DirectionalLight::SetDirectionalLightParameters(const Vect& dir, const Vect& amb, const Vect& dif, const Vect& sp)
{
	this->Light.Ambient = amb;
	this->Light.Diffuse = dif;
	this->Light.Specular = sp;
	this->Direction = dir;
}
