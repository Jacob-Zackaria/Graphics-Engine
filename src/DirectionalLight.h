#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H
#include "Matrix.h"
#include "LightType.h"
class DirectionalLight : public LightType
{
public:
	enum class Name
	{
		DIR_1,
		DIR_2,
		DIR_3,
		DIR_4,
		UNDEFINED
	};

	DirectionalLight(); // default constructor.
	DirectionalLight(const DirectionalLight&) = delete;				 // Copy constructor
	DirectionalLight(DirectionalLight&&) = default;                    // Move constructor
	DirectionalLight& operator=(const DirectionalLight&) & = default;  // Copy assignment operator
	DirectionalLight& operator=(DirectionalLight&&) & = default;       // Move assignment operator
	~DirectionalLight();		  							         // Destructor

	// Name
	const DirectionalLight::Name& GetName() const;
	void SetName(const DirectionalLight::Name newName);

	// Light
	const DirectionalLight::PhongADS& GetLight() const;
	void SetLight(const Vect newAmb, const Vect newDiffuse, const Vect newSpec);

	// Direction
	const Vect& GetDirection() const;
	void SetDirection(const Vect newDir);

	void SetDirectionalLightParameters(const Vect& dir, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));

private:
	DirectionalLight::Name name;
	PhongADS Light;
	Vect Direction;
};

#endif DIRECTIONAL_LIGHT_H