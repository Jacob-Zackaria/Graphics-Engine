#include "LODModel.h"
#include "GraphicObjectTextureLight.h"

LODModel::~LODModel()
{
}

LODModel::LODModel(GraphicObjectBase* newGraphBase, Matrix& newWorld)
	:
	name(LODModel::Name::UNDEFINED),
	pModelHead(nullptr),
	active(nullptr),
	pGraphBase(newGraphBase),
	mWorld(newWorld),
	pNext(nullptr),
	pPrev(nullptr)
{
}

void LODModel::Add(Model* newModel, float switchLength)
{
	newModel->SetLodLength(switchLength);
	this->privAddToFront(newModel, this->pModelHead);
	this->active = newModel;
}

//void LODModel::Remove()
//{
//}


const Vect LODModel::GetWorldPos() const
{
	return(this->mWorld.get(ROW_3));
}

const float LODModel::GetActiveLength() const
{
	return (this->active->GetLodLength());
}

const float LODModel::GetPrevActiveLength() const
{
	return (this->active->getLodPrev()->GetLodLength());
}

void LODModel::NextLod()
{
	this->active = this->active->getLodNext();
	this->pGraphBase->SetModel(this->active);
}

void LODModel::PrevLod()
{
	this->active = this->active->getLodPrev();
	this->pGraphBase->SetModel(this->active);
}

const bool LODModel::isNextLodNull() const
{
	return (this->active->getLodNext() == nullptr);
}

const bool LODModel::isPrevLodNull() const
{
	return (this->active->getLodPrev() == nullptr);
}

LODModel::Name LODModel::getName() const
{
	return this->name;
}

void LODModel::setName(LODModel::Name inName)
{
	this->name = inName;
}

//-----links----
void LODModel::setNext(LODModel* newNext)
{
	this->pNext = newNext;
}

void LODModel::setPrev(LODModel* newPrev)
{
	this->pPrev = newPrev;
}

LODModel* LODModel::getNext() const
{
	return (this->pNext);
}

LODModel* LODModel::getPrev() const
{
	return (this->pPrev);
}

void LODModel::privAddToFront(Model* node, Model*& head)
{
	assert(node);

	if (head == nullptr)
	{
		head = node;
		node->setLodNext(nullptr);
		node->setLodPrev(nullptr);
	}
	else
	{
		node->setLodNext(head);
		head->setLodPrev(node);
		head = node;
	}
}

void LODModel::privRemove(Model* pNewData, Model*& pHead)
{
	//Check if node to delete has a next or prev links.
	if (pNewData->getLodNext() == nullptr && pNewData->getLodPrev() == nullptr)
	{
		pHead = nullptr;
	}
	else if (pNewData->getLodPrev() == nullptr)    //Check if there is a prev link. 
	{
		pHead = pHead->getLodNext();
		pHead->setLodPrev(nullptr);
	}
	else if (pNewData->getLodNext() == nullptr)    //Check if there is a next link.
	{
		pNewData->getLodPrev()->setLodNext(nullptr);
	}
	else                    //If no next or prev links are present.
	{
		pNewData->getLodPrev()->setLodNext(pNewData->getLodNext());
		pNewData->getLodNext()->setLodPrev(pNewData->getLodPrev());
	}

	//Clear links of deleted node.
	pNewData->setLodNext(nullptr);
	pNewData->setLodPrev(nullptr);
}
