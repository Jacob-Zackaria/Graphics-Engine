#ifndef LERPER_H
#define LERPER_H

#include "Matrix.h"

struct VectPoints
{
	VectPoints(const VectPoints&) = delete;				 // Copy constructor
	VectPoints(VectPoints&&) = default;                    // Move constructor
	VectPoints& operator=(const VectPoints&) & = default;  // Copy assignment operator
	VectPoints& operator=(VectPoints&&) & = default;       // Move assignment operator
	~VectPoints() = default;
	VectPoints(Vect pNewPoint, const float newTime)
		:
		pNext(nullptr),
		pPrev(nullptr),
		point(pNewPoint),
		keyTime(newTime)
	{
	}

	//-----links----
	void setNext(VectPoints* newNext)
	{
		this->pNext = newNext;
	}
	void setPrev(VectPoints* newPrev)
	{
		this->pPrev = newPrev;
	}
	VectPoints* getNext() const
	{
		return (this->pNext);
	}
	VectPoints* getPrev() const
	{
		return (this->pPrev);
	}

	const Vect& getPoint() const
	{
		return (this->point);
	}
	
	const float& getTime() const
	{
		return (this->keyTime);
	}

private:
	VectPoints* pNext;
	VectPoints* pPrev;

	Vect point;
	float keyTime;
};

class Lerper
{
public:
	Lerper(const Lerper&) = delete;				 // Copy constructor
	Lerper(Lerper&&) = default;                    // Move constructor
	Lerper& operator=(const Lerper&) & = default;  // Copy assignment operator
	Lerper& operator=(Lerper&&) & = default;       // Move assignment operator
	~Lerper();
	Lerper() = default;

	void Add(Vect pNewPoint, const float newTime);
	const Vect Update(const float gameTime);
	//static void Remove();

private:
	
	void privAddToEnd(VectPoints* node, VectPoints*& head);
	void privRemove(VectPoints* pNewData, VectPoints*& pHead);

	VectPoints* active;
};

#endif LERPER_H