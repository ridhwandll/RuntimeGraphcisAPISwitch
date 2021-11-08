#pragma once
#define WIN32_LEAN_AND_MEAN
#include "Defines.hpp"
#include <windows.h> // PCH goes brrrrrrr

enum class RenderAPIType
{
	Vulkan,
	DirectX
};
static RenderAPIType GAPIType = RenderAPIType::DirectX;

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

template<auto Func>
using FunctionType = decltype(Func);

using TextureRenderPtr = FunctionType<&TextureImpl::Render>;
using TextureBindPtr = FunctionType<&TextureImpl::Bind>;
using TextureGetCountPtr = FunctionType<&TextureImpl::GetCount>;

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

		PFNGetRenderMethod	pfnGetRenderMethod = GetFunc<PFNGetRenderMethod>(mDllHandle, "GetRenderFunction");
		PFNGetBindMethod	pfnGetBindMethod = GetFunc<PFNGetBindMethod>(mDllHandle, "GetBindFunction");
		PFNGetCountMethod	pfnGetCountMethod = GetFunc<PFNGetCountMethod>(mDllHandle, "GetCountFunction");

		mInternalRender = (pfnGetRenderMethod());
		mInternalBind = (pfnGetBindMethod());
		mInternalGetCount = (pfnGetCountMethod());

		auto mPfnCreateTexture = GetFunc<PFNCreateTexture>(mDllHandle, "CreateTexture");;
		mImpl = mPfnCreateTexture();
	}
	void Destroy()
	{
		auto mPfnDeleteTexture = GetFunc<PFNDeleteTexture>(mDllHandle, "DeleteTexture");
		mPfnDeleteTexture(mImpl);
		FreeLibrary(mDllHandle);
	}
	void Render()
	{
		(*mImpl.*mInternalRender)();
	}
	void Bind(int commandIndex)
	{
		(*mImpl.*mInternalBind)(commandIndex);
	}
	int GetCount()
	{
		return (mImpl->*mInternalGetCount)();
	}
private:
	HMODULE mDllHandle;
	TextureImpl* mImpl;
	TextureRenderPtr mInternalRender;
	TextureBindPtr mInternalBind;
	TextureGetCountPtr mInternalGetCount;
};