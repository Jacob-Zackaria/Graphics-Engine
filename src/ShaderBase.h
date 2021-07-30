// ShaderBase
// AB, June 2016

#ifndef _ShaderBase
#define _ShaderBase

#include <d3dcommon.h>
#include "Matrix.h"
#include <string>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11DeviceContext;
struct D3D11_INPUT_ELEMENT_DESC;

class ShaderBase : public Align16
{
public:

	ShaderBase(const ShaderBase&) = delete;				  // Copy constructor
	ShaderBase(ShaderBase&&) = delete;                    // Move constructor
	ShaderBase& operator=(const ShaderBase&) & = delete;  // Copy assignment operator
	ShaderBase& operator=(ShaderBase&&) & = delete;       // Move assignment operator
	ShaderBase() = delete;
	virtual ~ShaderBase();

	virtual void SetToContext() = 0;
	ID3D11DeviceContext* GetContext();

	const bool CompareName(char* newName);
	char* getName() const;
	void setName(char* name);

	//-----links----
	void setNext(ShaderBase* newNext);
	void setPrev(ShaderBase* newPrev);
	ShaderBase* getNext() const;
	ShaderBase* getPrev() const;

protected:
	ID3D11Device* GetDevice(); 

	ShaderBase(ID3D11Device* device, ID3D11DeviceContext* newContext, WCHAR* filename, LPCSTR vsModel = "vs_4_0", LPCSTR psModel = "ps_4_0");
	void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layoutdesc, UINT size);
	void SetContext(ID3D11DeviceContext* devcon);
	void SetToContext_VS_PS_InputLayout();

private:
	char* name;
	// links
	ShaderBase* pNext;
	ShaderBase* pPrev;

	// Store these pointers to avoid passing them in every method
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mContext;

	// Actual objects managed by this class
	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;
	ID3D11InputLayout* mpVertexLayout;

	// Needed for building shaders
	ID3DBlob* pVSBlob;
	ID3DBlob* pPSBlob;

	HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	void privBuildShaders(WCHAR* filename, LPCSTR vsModel, LPCSTR psModel);
};


#endif _ShaderBase