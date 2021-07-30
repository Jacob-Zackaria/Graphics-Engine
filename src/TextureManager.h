#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "Texture.h"

class TextureManager
{
public:
	TextureManager(const TextureManager&) = delete;				 // Copy constructor
	TextureManager(TextureManager&&) = default;                    // Move constructor
	TextureManager& operator=(const TextureManager&) & = default;  // Copy assignment operator
	TextureManager& operator=(TextureManager&&) & = default;       // Move assignment operator
	~TextureManager();


	static void Create();
	static void Destroy();
	static void Add(Texture* pNewTexture, Texture::Name newName);
	//static void Remove();

private:
	TextureManager() = default;
	static TextureManager* privGetInstance();

	void privAddToFront(Texture* node, Texture*& head);
	void privRemove(Texture* pNewData, Texture*& pHead);

	Texture* active;

};

#endif TEXTURE_MANAGER_H