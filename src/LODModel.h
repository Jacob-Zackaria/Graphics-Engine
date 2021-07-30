#ifndef LOD_MODEL_H
#define LOD_MODEL_H
#include "Model.h"
#include "GraphicObjectBase.h"

class LODModel
{
public:

	enum class Name
	{
		TROLL,
		SMALL_TROLL,
		SEAGULL,
		WIZARD,
		BOAT,
		FLOWER,
		FISH,
		UFO,
		ORIGAMI_TREE,
		UNDEFINED
	};

	LODModel(const LODModel&) = delete;				 // Copy constructor
	LODModel(LODModel&&) = default;                    // Move constructor
	LODModel& operator=(const LODModel&) & = default;  // Copy assignment operator
	LODModel& operator=(LODModel&&) & = default;       // Move assignment operator
	~LODModel();
	LODModel(GraphicObjectBase* newGraphBase, Matrix& newWorld);

	void Add(Model* newModel, float switchLength = 0.0f);
	//void Remove();

	const Vect GetWorldPos() const;
	const float GetActiveLength() const;
	const float GetPrevActiveLength() const;

	void NextLod();
	void PrevLod();

	const bool isNextLodNull() const;
	const bool isPrevLodNull() const;

	Name getName() const;
	void setName(LODModel::Name name);

	//-----links----
	void setNext(LODModel* newNext);
	void setPrev(LODModel* newPrev);
	LODModel* getNext() const;
	LODModel* getPrev() const;

private:
	void privAddToFront(Model* node, Model*& head);
	void privRemove(Model* pNewData, Model*& pHead);

	LODModel::Name name;
	Model* pModelHead;
	Model* active;
	GraphicObjectBase* pGraphBase;
	Matrix mWorld;

	// links
	LODModel* pNext;
	LODModel* pPrev;
};

#endif LOD_MODEL_H