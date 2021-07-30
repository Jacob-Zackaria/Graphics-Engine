#include "Lerper.h"
#include "VectApp.h"

Lerper::~Lerper()
{
	VectPoints* pLink = this->active;
	VectPoints* pTmp = nullptr;
	while (pLink != nullptr)
	{
		pTmp = pLink;
		pLink = pLink->getNext();

		delete pTmp;
	}
}


void Lerper::Add(Vect pNewPoint, const float newTime)
{
	VectPoints* pNewSplinePoint = new VectPoints(pNewPoint, newTime);

	// Now add it to the manager
	this->privAddToEnd(pNewSplinePoint, this->active);
}

const Vect Lerper::Update(const float gameTime)
{
	// head
	VectPoints* pTmp = this->active;

	if (pTmp == nullptr)
	{
		return Vect(1.0f,1.0f,1.0f);
	}

	// loop according to game Time.
	while (gameTime >= pTmp->getTime() && pTmp->getNext() != nullptr)
	{
		pTmp = pTmp->getNext();
	}

	if (gameTime >= pTmp->getTime() && pTmp->getNext() == nullptr)
	{
		return Vect(1.0f, 1.0f, 1.0f);
	}

	VectPoints* pA = pTmp->getPrev();
	VectPoints* pB = pTmp;

	float tS = (gameTime - pA->getTime()) / (pB->getTime() - pA->getTime());

	Vect interpolatedPosition;
	VectApp::Lerp(interpolatedPosition, pA->getPoint(), pB->getPoint(), tS);

	return (interpolatedPosition);
}

void Lerper::privAddToEnd(VectPoints* node, VectPoints*& head)
{
	assert(node);

	if (head == nullptr)
	{
		head = node;
		node->setPrev(nullptr);
	}
	else
	{
		VectPoints* pTmp = head;
		while (pTmp->getNext() != nullptr)
		{
			pTmp = pTmp->getNext();
		}
		node->setPrev(pTmp);
		pTmp->setNext(node);
	}
	node->setNext(nullptr);
}

void Lerper::privRemove(VectPoints* pNewData, VectPoints*& pHead)
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
