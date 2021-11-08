#pragma once
#define WIN32_LEAN_AND_MEAN
#include "Defines.hpp"
#include <windows.h> // PCH goes brrrrrrr

enum class RenderAPIType
{
	Vulkan,
	DirectX
};
static RenderAPIType GAPIType = RenderAPIType::Vulkan;

void SetAPI(RenderAPIType type)
{
	GAPIType = type;
}

class TextureImpl
{
public:
	void Render();
	void Bind(int commandIndex);
	int GetCount();
};

using TextureRenderPtr = decltype(&TextureImpl::Render);
using TextureBindPtr = decltype(&TextureImpl::Bind);
using TextureGetCountPtr = decltype(&TextureImpl::GetCount);

using PFNGetRenderMethod = TextureRenderPtr(*)();
using PFNGetBindMethod = TextureBindPtr(*)();
using PFNGetCountMethod = TextureGetCountPtr(*)();

using PFNCreateTexture = TextureImpl * (*)();
using PFNDeleteTexture = void(*)(TextureImpl*);

template<class FuncSig>
FuncSig GetFunc(HMODULE hDll, const char* signature)
{
	FuncSig func = (FuncSig)GetProcAddress(hDll, signature);
	return func;
}

class Texture
{
public:
	void Initialize()
	{
		if (GAPIType == RenderAPIType::Vulkan)
			mDllHandle = LoadLibrary("VulkanImpl.dll");
		else if (GAPIType == RenderAPIType::DirectX)
			mDllHandle = LoadLibrary("DirectX12Impl.dll");

		mPfnCreateTexture = GetFunc<PFNCreateTexture>(mDllHandle, "CreateTexture");;
		mPfnDeleteTexture = GetFunc<PFNDeleteTexture>(mDllHandle, "DeleteTexture");

		PFNGetRenderMethod	pfnGetRenderMethod = GetFunc<PFNGetRenderMethod>(mDllHandle, "GetRenderFunction");
		PFNGetBindMethod	pfnGetBindMethod = GetFunc<PFNGetBindMethod>(mDllHandle, "GetBindFunction");
		PFNGetCountMethod	pfnGetCountMethod = GetFunc<PFNGetCountMethod>(mDllHandle, "GetCountFunction");

		mInternalRender = (pfnGetRenderMethod());
		mInternalBind = (pfnGetBindMethod());
		mInternalGetCount = (pfnGetCountMethod());
		GET_WIN32_LAST_ERROR;
		mImpl = mPfnCreateTexture();
	}
	void Destroy()
	{
		mPfnDeleteTexture(mImpl);
		FreeLibrary(mDllHandle);
	}
	void Render()
	{
		(mImpl->*mInternalRender)();
	}
	void Bind(int commandIndex)
	{
		(mImpl->*mInternalBind)(commandIndex);
	}
	int GetCount()
	{
		return (mImpl->*mInternalGetCount)();
	}
private:
	HMODULE mDllHandle = nullptr; // Should not be per texture(In this demo you can't create more than one texture kekw), should be loaded once at engine startup(I am lazy)
	TextureImpl* mImpl;
	TextureRenderPtr mInternalRender;
	TextureBindPtr mInternalBind;
	TextureGetCountPtr mInternalGetCount;

	PFNCreateTexture	mPfnCreateTexture;
	PFNDeleteTexture	mPfnDeleteTexture;
};