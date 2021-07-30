#include "ModelManager.h"

ModelManager::~ModelManager()
{
}

ModelManager* ModelManager::privGetInstance()
{
	static ModelManager lodMan;
	return &lodMan;
}

void ModelManager::Create()
{
	// Get the instance to the manager
	ModelManager* pTexMan = ModelManager::privGetInstance();
	assert(pTexMan);

	//unused variable.
	pTexMan;
}

void ModelManager::Destroy()
{
	// Get the instance to the manager
	ModelManager* pModelMan = ModelManager::privGetInstance();
	assert(pModelMan);

	Model* pLink = pModelMan->active;
	Model* pTmp = nullptr;
	while (pLink != nullptr)
	{
		pTmp = pLink;
		pLink = pLink->getNext();

		delete pTmp;
	}
}

void ModelManager::Add(Model* pNewTexture, Model::Name newName)
{
	// Get the instance to the manager
	ModelManager* pModelMan = ModelManager::privGetInstance();
	assert(pModelMan);


	assert(pNewTexture);
	pNewTexture->setName(newName);

	// Now add it to the manager
	pModelMan->privAddToFront(pNewTexture, pModelMan->active);
}

void ModelManager::privAddToFront(Model* node, Model*& head)
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

void ModelManager::privRemove(Model* pNewData, Model*& pHead)
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

