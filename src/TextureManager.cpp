#include "TextureManager.h"

TextureManager::~TextureManager()
{
}

TextureManager* TextureManager::privGetInstance()
{
	static TextureManager lodMan;
	return &lodMan;
}

void TextureManager::Create()
{
	// Get the instance to the manager
	TextureManager* pTexMan = TextureManager::privGetInstance();
	assert(pTexMan);

	//unused variable.
	pTexMan;
}

void TextureManager::Destroy()
{
	// Get the instance to the manager
	TextureManager* pTexMan = TextureManager::privGetInstance();
	assert(pTexMan);

	Texture* pLink = pTexMan->active;
	Texture* pTmp = nullptr;
	while (pLink != nullptr)
	{
		pTmp = pLink;
		pLink = pLink->getNext();

		delete pTmp;
	}
}

void TextureManager::Add(Texture* pNewTexture, Texture::Name newName)
{
	// Get the instance to the manager
	TextureManager* pTexMan = TextureManager::privGetInstance();
	assert(pTexMan);


	assert(pNewTexture);
	pNewTexture->setName(newName);

	// Now add it to the manager
	pTexMan->privAddToFront(pNewTexture, pTexMan->active);
}

void TextureManager::privAddToFront(Texture* node, Texture*& head)
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

void TextureManager::privRemove(Texture* pNewData, Texture*& pHead)
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

