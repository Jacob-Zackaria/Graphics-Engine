#include "CamSplineControl.h"
#include "VectApp.h"

CamSplineControl::~CamSplineControl()
{
}

CamSplineControl* CamSplineControl::privGetInstance()
{
	static CamSplineControl lodMan;
	return &lodMan;
}

void CamSplineControl::Create()
{
	// Get the instance to the manager
	CamSplineControl* pSplineMan = CamSplineControl::privGetInstance();
	assert(pSplineMan);

	//unused variable.
	pSplineMan;
}

void CamSplineControl::Destroy()
{
	// Get the instance to the manager
	CamSplineControl* pSplineMan = CamSplineControl::privGetInstance();
	assert(pSplineMan);

	SplinePoints* pLink = pSplineMan->active;
	SplinePoints* pTmp = nullptr;
	while (pLink != nullptr)
	{
		pTmp = pLink;
		pLink = pLink->getNext();

		delete pTmp;
	}
}

void CamSplineControl::Add(Vect pNewPoint, Vect pNewLookAt, const float newTime)
{
	// Get the instance to the manager
	CamSplineControl* pSplineMan = CamSplineControl::privGetInstance();
	assert(pSplineMan);

	SplinePoints* pNewSplinePoint = new SplinePoints(pNewPoint, pNewLookAt, newTime);

	// Now add it to the manager
	pSplineMan->privAddToEnd(pNewSplinePoint, pSplineMan->active);
}

void CamSplineControl::Update(Camera& mCam, const float gameTime)
{
	// Get the instance to the manager
	CamSplineControl* pSplineMan = CamSplineControl::privGetInstance();
	assert(pSplineMan);

	// head
	SplinePoints* pTmp = pSplineMan->active;

	if (pTmp == nullptr)
	{
		return;
	}

	// loop according to game Time.
	while (gameTime >= pTmp->getTime() && pTmp->getNext() != nullptr)
	{
		pTmp = pTmp->getNext();
	}

	if (gameTime >= pTmp->getTime() && pTmp->getNext() == nullptr)
	{
		return;
	}

	SplinePoints* pA = pTmp->getPrev();
	SplinePoints* pB = pTmp;

	float tS = (gameTime - pA->getTime()) / (pB->getTime() - pA->getTime());

	Vect interpolatedPosition, interpolatedLook;
	VectApp::Lerp(interpolatedPosition, pA->getPoint(), pB->getPoint(), tS);
	VectApp::Lerp(interpolatedLook, pA->getLookAt(), pB->getLookAt(), tS);

	mCam.setOrientAndPosition(Vect(0, 1, 0), interpolatedLook, interpolatedPosition);
}

void CamSplineControl::privAddToEnd(SplinePoints* node, SplinePoints*& head)
{
	assert(node);

	if (head == nullptr)
	{
		head = node;
		node->setPrev(nullptr);
	}
	else
	{
		SplinePoints* pTmp = head;
		while (pTmp->getNext() != nullptr)
		{
			pTmp = pTmp->getNext();
		}
		node->setPrev(pTmp);
		pTmp->setNext(node);
	}
	node->setNext(nullptr);
}

void CamSplineControl::privRemove(SplinePoints* pNewData, SplinePoints*& pHead)
{
	//Check if node to delete has a next or prev links.
	if (pNewData->getNext() == nullptr && pNewData->getPrev() == nullptr)
	{
		pHead = nullptr;
	}
	else if (pNewData->getPrev() == nullptr)    //Check if there is a prev link. 
	{
		pHead = pHead->getNext();
		pHead->setPrev(nullptr);
	}
	else if (pNewData->getNext() == nullptr)    //Check if there is a next link.
	{
		pNewData->getPrev()->setNext(nullptr);
	}
	else                    //If no next or prev links are present.
	{
		pNewData->getPrev()->setNext(pNewData->getNext());
		pNewData->getNext()->setPrev(pNewData->getPrev());
	}

	//Clear links of deleted node.
	pNewData->setNext(nullptr);
	pNewData->setPrev(nullptr);
}
