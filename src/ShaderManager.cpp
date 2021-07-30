#include "ShaderManager.h"

ShaderManager::~ShaderManager()
{
}

ShaderManager* ShaderManager::privGetInstance()
{
	static ShaderManager lodMan;
	return &lodMan;
}

void ShaderManager::Create()
{
	// Get the instance to the manager
	ShaderManager* pTexMan = ShaderManager::privGetInstance();
	assert(pTexMan);

	//unused variable.
	pTexMan;
}

void ShaderManager::Destroy()
{
	// Get the instance to the manager
	ShaderManager* pShaderMan = ShaderManager::privGetInstance();
	assert(pShaderMan);

	ShaderBase* pLink = pShaderMan->active;
	ShaderBase* pTmp = nullptr;
	while (pLink != nullptr)
	{
		pTmp = pLink;
		pLink = pLink->getNext();

		delete pTmp;
	}
}

void ShaderManager::Add(ShaderBase* pNewTexture, char* newName)
{
	// Get the instance to the manager
	ShaderManager* pShaderMan = ShaderManager::privGetInstance();
	assert(pShaderMan);


	assert(pNewTexture);
	pNewTexture->setName(newName);

	// Now add it to the manager
	pShaderMan->privAddToFront(pNewTexture, pShaderMan->active);
}

ShaderBase* ShaderManager::Find(char* newName)
{
	// Get the instance to the manager
	ShaderManager* pShaderMan = ShaderManager::privGetInstance();
	assert(pShaderMan);

	ShaderBase* pLink = pShaderMan->active;
	while (pLink != nullptr)
	{
		if (pLink->CompareName(newName))
		{
			return pLink;
		}
		pLink = pLink->getNext();
	}

	return nullptr;
}

void ShaderManager::privAddToFront(ShaderBase* node, ShaderBase*& head)
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

void ShaderManager::privRemove(ShaderBase* pNewData, ShaderBase*& pHead)
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
