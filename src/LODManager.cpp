#include "LODManager.h"

LODManager::~LODManager()
{
}

LODManager* LODManager::privGetInstance()
{
	static LODManager lodMan;
	return &lodMan;
}

void LODManager::Create()
{
	// Get the instance to the manager
	LODManager* plodMan = LODManager::privGetInstance();
	assert(plodMan);

	//unused variable.
	plodMan;
}

void LODManager::Destroy()
{
	// Get the instance to the manager
	LODManager* plodMan = LODManager::privGetInstance();
	assert(plodMan);

	LODModel* pLink = plodMan->active;
	LODModel* pTmp = nullptr;
	while (pLink != nullptr)
	{
		pTmp = pLink;
		pLink = pLink->getNext();

		delete pTmp;
	}
}

void LODManager::Add(LODModel* pNewModel, LODModel::Name newName)
{
	// Get the instance to the manager
	LODManager* plodMan = LODManager::privGetInstance();
	assert(plodMan);


	assert(pNewModel);
	pNewModel->setName(newName);

	// Now add it to the manager
	plodMan->privAddToFront(pNewModel, plodMan->active);
}

void LODManager::Update(Camera& pnewCam)
{
	// Get the instance to the manager
	LODManager* plodMan = LODManager::privGetInstance();
	assert(plodMan);

	// Get camera position.
	Vect camPos;
	pnewCam.getPos(camPos);

	LODModel* pLink = plodMan->active;
	while (pLink != nullptr)
	{
		// distance of Cmaera from Model World Position.
		Vect newVec = camPos - pLink->GetWorldPos();
		float dist = newVec.mag();


		if ((!pLink->isNextLodNull()) && (dist > pLink->GetActiveLength()))
		{
			// load low poly
			pLink->NextLod();
		}
		else if ((!pLink->isPrevLodNull()) && (dist < pLink->GetPrevActiveLength()))
		{
			// load high poly
			pLink->PrevLod();
		}

		pLink = pLink->getNext();
	}
}

void LODManager::privAddToFront(LODModel* node, LODModel*& head)
{
	assert(node);

	if (head == nullptr)
	{
		head = node;
		node->setNext(nullptr);
		node->setPrev(nullptr);
	}
	else
	{
		node->setNext(head);
		head->setPrev(node);
		head = node;
	}
}

void LODManager::privRemove(LODModel* pNewData, LODModel*& pHead)
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
