#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include "Model.h"

class ModelManager
{
public:
	ModelManager(const ModelManager&) = delete;				 // Copy constructor
	ModelManager(ModelManager&&) = default;                    // Move constructor
	ModelManager& operator=(const ModelManager&) & = default;  // Copy assignment operator
	ModelManager& operator=(ModelManager&&) & = default;       // Move assignment operator
	~ModelManager();


	static void Create();
	static void Destroy();
	static void Add(Model* pNewModel, Model::Name newName);
	//static void Remove();

private:
	ModelManager() = default;
	static ModelManager* privGetInstance();

	void privAddToFront(Model* node, Model*& head);
	void privRemove(Model* pNewData, Model*& pHead);

	Model* active;

};

#endif MODEL_MANAGER_H