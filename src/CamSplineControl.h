#ifndef CAM_SPLINE_CONTROL_H
#define CAM_SPLINE_CONTROL_H

#include "Matrix.h"
#include "Camera.h"

struct SplinePoints
{
	SplinePoints(const SplinePoints&) = delete;				 // Copy constructor
	SplinePoints(SplinePoints&&) = default;                    // Move constructor
	SplinePoints& operator=(const SplinePoints&) & = default;  // Copy assignment operator
	SplinePoints& operator=(SplinePoints&&) & = default;       // Move assignment operator
	~SplinePoints() = default;
	SplinePoints(Vect pNewPoint, Vect newLookAt, const float newTime)
		:
		pNext(nullptr),
		pPrev(nullptr),
		point(pNewPoint),
		lookAt(newLookAt),
		keyTime(newTime)
	{
	}

	//-----links----
	void setNext(SplinePoints* newNext)
	{
		this->pNext = newNext;
	}
	void setPrev(SplinePoints* newPrev)
	{
		this->pPrev = newPrev;
	}
	SplinePoints* getNext() const
	{
		return (this->pNext);
	}
	SplinePoints* getPrev() const
	{
		return (this->pPrev);
	}

	const Vect& getPoint() const
	{
		return (this->point);
	}
	const Vect& getLookAt() const
	{
		return (this->lookAt);
	}

	const float& getTime() const
	{
		return (this->keyTime);
	}

private:
	SplinePoints* pNext;
	SplinePoints* pPrev;

	Vect point;
	Vect lookAt;
	float keyTime;
};

class CamSplineControl
{
public:
	CamSplineControl(const CamSplineControl&) = delete;				 // Copy constructor
	CamSplineControl(CamSplineControl&&) = default;                    // Move constructor
	CamSplineControl& operator=(const CamSplineControl&) & = default;  // Copy assignment operator
	CamSplineControl& operator=(CamSplineControl&&) & = default;       // Move assignment operator
	~CamSplineControl();


	static void Create();
	static void Destroy();
	static void Add(Vect pNewPoint, Vect pNewLookAt, const float newTime);
	static void Update(Camera& mCam, const float gameTime);
	//static void Remove();

private:
	CamSplineControl() = default;
	static CamSplineControl* privGetInstance();

	void privAddToEnd(SplinePoints* node, SplinePoints*& head);
	void privRemove(SplinePoints* pNewData, SplinePoints*& pHead);

	SplinePoints* active;
};

#endif CAM_SPLINE_CONTROL_H