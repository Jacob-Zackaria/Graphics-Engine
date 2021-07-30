#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H
#include "Matrix.h"
#include "LightType.h"
class SpotLight : public LightType
{
public:
	enum class Name
	{
		SPOT_1,
		SPOT_2,
		SPOT_3,
		SPOT_4,
		UNDEFINED
	};
	SpotLight(); // default constructor.
	SpotLight(const SpotLight&) = delete;				 // Copy constructor
	SpotLight(SpotLight&&) = default;                    // Move constructor
	SpotLight& operator=(const SpotLight&) & = default;  // Copy assignment operator
	SpotLight& operator=(SpotLight&&) & = default;       // Move assignment operator
	~SpotLight();		  							         // Destructor

	// Accessors>>
	// Position
	const Vect& GetPosition() const;
	void SetPosition(const Vect newPos);
	// Name
	const SpotLight::Name GetName() const;
	void SetName(const SpotLight::Name newName);

	// Light
	const SpotLight::PhongADS& GetLight() const;
	void SetLight(const Vect newAmb, const Vect newDiffuse, const Vect newSpec);

	// Attenuation
	const Vect& GetAttenuation() const;
	void SetAttenuation(const Vect newAtten);

	// Direction
	const Vect& GetDirection() const;
	void SetDirection(const Vect newDir);

	// Range
	const float& GetRange() const;
	void SetRange(const float newRange);

	// SpotExposure
	const float& GetExposure() const;
	void SetExposure(const float newExp);

	void SetSpotLightParameters(const Vect& pos, float r, const Vect& att, const Vect& dir, float spotExp, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));

private:
	SpotLight::Name name;
	PhongADS Light;
	Vect Position;
	Vect Attenuation;
	Vect Direction;
	float SpotExp;
	float Range;
};

#endif POINT_LIGHT_H