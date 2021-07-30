#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "ShaderBase.h"

class ShaderManager
{
public:
	ShaderManager(const ShaderManager&) = delete;				 // Copy constructor
	ShaderManager(ShaderManager&&) = default;                    // Move constructor
	ShaderManager& operator=(const ShaderManager&) & = default;  // Copy assignment operator
	ShaderManager& operator=(ShaderManager&&) & = default;       // Move assignment operator
	~ShaderManager();


	static void Create();
	static void Destroy();
	static void Add(ShaderBase* pNewShader, char* newName);
	//static void Remove();
	static ShaderBase* Find(char* newName);

private:
	ShaderManager() = default;
	static ShaderManager* privGetInstance();

	void privAddToFront(ShaderBase* node, ShaderBase*& head);
	void privRemove(ShaderBase* pNewData, ShaderBase*& pHead);

	ShaderBase* active;

};

#endif SHADER_MANAGER_H