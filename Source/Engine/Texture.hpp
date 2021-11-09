#pragma once
#include "Defines.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h> // PCH goes brrrrrrr
#define DLL_HANDLE HMODULE
#else
#include <dlfcn.h> // LibDL
#define DLL_HANDLE void*
#define GetProcAddress dlsym
#endif

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
FuncSig GetFunc(DLL_HANDLE hDll, const char* signature)
{
	FuncSig func = (FuncSig)GetProcAddress(hDll, signature);
	return func;
}

class Texture
{
public:
	void Initialize()
	{
#ifdef _WIN32
		if (GAPIType == RenderAPIType::Vulkan)
			mDllHandle = LoadLibrary("VulkanImpl.dll");
		else if (GAPIType == RenderAPIType::DirectX)
			mDllHandle = LoadLibrary("DirectX12Impl.dll");
#else	
		if (GAPIType == RenderAPIType::Vulkan)
			mDllHandle = dlopen("./libVulkanImpl.so", RTLD_NOW);
		else if (GAPIType == RenderAPIType::DirectX)
			mDllHandle = dlopen("./libDirectX12Impl.so", RTLD_NOW);
#endif

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
#ifdef _WIN32
		FreeLibrary(mDllHandle);
#else
		dlclose(mDllHandle);
#endif
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
	DLL_HANDLE mDllHandle;
	TextureImpl* mImpl;
	TextureRenderPtr mInternalRender;
	TextureBindPtr mInternalBind;
	TextureGetCountPtr mInternalGetCount;
};
