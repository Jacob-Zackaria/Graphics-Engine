#ifndef LIGHT_TYPE_H
#define LIGHT_TYPE_H
#include "Matrix.h"

class LightType
{
protected:
	LightType(); // default constructor.
	LightType(const LightType&) = delete;				 // Copy constructor
	LightType(LightType&&) = default;                    // Move constructor
	LightType& operator=(const LightType&) & = default;  // Copy assignment operator
	LightType& operator=(LightType&&) & = default;       // Move assignment operator
	virtual ~LightType() = default;		  							         // Destructor

public:
	struct PhongADS
	{
		Vect Ambient;
		Vect Diffuse;
		Vect Specular;
	};


	LightType* next;
	LightType* prev;
};

#endif LIGHT_TYPE_H