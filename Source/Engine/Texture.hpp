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

#include <string>
#include <vector>


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

class VirtualTexture
{
public:
	virtual ~VirtualTexture() = default;
	virtual std::string Render() = 0;
	virtual std::string GetPath() const = 0;
	virtual int GetCount() const = 0;
};

// VIRTUAL ---------------------------
class DX12VirtualTexture : public VirtualTexture
{
public:
	DX12VirtualTexture() = default;
	~DX12VirtualTexture() = default;
	virtual std::string Render() override
	{
		std::vector<std::string> paths;
		for (int i = 0; i < 10; i++)
			paths.push_back(mPath.append(std::to_string(mCount) + mPath.at(0) + mPath.at(1) + mPath.at(2)));
		return paths.at(0);
	}
	virtual std::string GetPath() const override { return mPath; };
	virtual int GetCount() const override { return mCount + 1; };
private:
	std::string mPath = "AHUGEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEPATHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
	int mCount = 9;
};

class VulkanVirtualTexture : public VirtualTexture
{
public:
	VulkanVirtualTexture() = default;
	~VulkanVirtualTexture() = default;
	virtual std::string Render() override
	{
		std::vector<std::string> paths;
		for (int i = 0; i < 10; i++)
			paths.push_back(mPath.append(std::to_string(mCount) + mPath.at(0) + mPath.at(1) + mPath.at(2)));
		return paths.at(0);
	}
	virtual std::string GetPath() const override { return mPath; };
	virtual int GetCount() const override { return mCount + 1; };
private:
	std::string mPath = "AHUGEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEPATHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
	int mCount = 9;
};

VirtualTexture* CreateVirtualTexture()
{
	switch (GAPIType)
	{
	case RenderAPIType::Vulkan: return new VulkanVirtualTexture;
	case RenderAPIType::DirectX: return new DX12VirtualTexture;
	}
	return nullptr;
}

// NON VIRTUAl ---------------------------
class TextureImpl
{
public:
	TextureImpl();
	std::string Render()
	{
		std::vector<std::string> paths;
		for (int i = 0; i < 10; i++)
			paths.push_back(mPath.append(std::to_string(mCount) + mPath.at(0) + mPath.at(1) + mPath.at(2)));
		return paths.at(0);
	}
	std::string GetPath() const { return mPath; };
	int GetCount() const { return mCount + 1; }
private:
	std::string mPath{};
	int mCount = 9;
};

template<auto Func>
using FunctionType = decltype(Func);

using TextureRenderPtr = FunctionType<&TextureImpl::Render>;
using TextureGetPathPtr = FunctionType<&TextureImpl::GetPath>;
using TextureGetCountPtr = FunctionType<&TextureImpl::GetCount>;

using PFNGetRenderMethod = TextureRenderPtr(*)();
using PFNGetGetPathMethod = TextureGetPathPtr(*)();
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
		PFNGetGetPathMethod	pfnGetGetPathMethod = GetFunc<PFNGetGetPathMethod>(mDllHandle, "GetGetPathFunction");
		PFNGetCountMethod	pfnGetCountMethod = GetFunc<PFNGetCountMethod>(mDllHandle, "GetCountFunction");

		mInternalRender = (pfnGetRenderMethod());
		mInternalGetPath = (pfnGetGetPathMethod());
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
	std::string Render()
	{
		return (*mImpl.*mInternalRender)();
	}
	std::string GetPath() const
	{
		return (*mImpl.*mInternalGetPath)();
	}
	int GetCount()
	{
		return (mImpl->*mInternalGetCount)();
	}
private:
	DLL_HANDLE mDllHandle;
	TextureImpl* mImpl;
	TextureRenderPtr mInternalRender;
	TextureGetPathPtr mInternalGetPath;
	TextureGetCountPtr mInternalGetCount;
};
