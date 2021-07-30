#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H
#include "Matrix.h"
#include "LightType.h"
class PointLight : public LightType
{
public:
	enum class Name
	{
		POINT_1,
		POINT_2,
		POINT_3,
		POINT_4,
		UNDEFINED
	};
	PointLight(); // default constructor.
	PointLight(const PointLight&) = delete;				 // Copy constructor
	PointLight(PointLight&&) = default;                    // Move constructor
	PointLight& operator=(const PointLight&) & = default;  // Copy assignment operator
	PointLight& operator=(PointLight&&) & = default;       // Move assignment operator
	~PointLight();		  							         // Destructor

	// Position
	const Vect& GetPosition() const;
	void SetPosition(const Vect newPos);
	// Name
	const PointLight::Name GetName() const;
	void SetName(const PointLight::Name newName);

	// Light
	const PointLight::PhongADS& GetLight() const;
	void SetLight(const Vect newAmb, const Vect newDiffuse, const Vect newSpec);

	// Attenuation
	const Vect& GetAttenuation() const;
	void SetAttenuation(const Vect newAtten);

	// Range
	const float& GetRange() const;
	void SetRange(const float newRange);

	void SetPointLightParameters(const Vect& pos, float r, const Vect& att, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));

private:
	PointLight::Name name;
	PhongADS Light;
	Vect Position;
	Vect Attenuation;
	float Range;
};

#endif POINT_LIGHT_H