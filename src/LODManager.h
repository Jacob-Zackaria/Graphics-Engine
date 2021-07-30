#ifndef LOD_MANAGER_H
#define LOD_MANAGER_H

#include "LODModel.h"
#include "Camera.h"

class LODManager
{
public:
	LODManager(const LODManager&) = delete;				 // Copy constructor
	LODManager(LODManager&&) = default;                    // Move constructor
	LODManager& operator=(const LODManager&) & = default;  // Copy assignment operator
	LODManager& operator=(LODManager&&) & = default;       // Move assignment operator
	~LODManager();
	

	static void Create();
	static void Destroy();
	static void Add(LODModel* pNewModel, LODModel::Name newName);
	//static void Remove();
	static void Update(Camera& pNewCam);

private:
	LODManager() = default;
	static LODManager* privGetInstance();

	void privAddToFront(LODModel* node, LODModel*& head);
	void privRemove(LODModel* pNewData, LODModel*& pHead);

	LODModel* active;
	
};

#endif LOD_MANAGER_H