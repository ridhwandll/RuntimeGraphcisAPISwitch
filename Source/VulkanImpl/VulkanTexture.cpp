#include "../Engine/Texture.hpp" // I don't like this ../, but I am lazy

extern "C"
{
	TextureImpl::TextureImpl()
	{
		mPath = "AHUGEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEPATHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
	}

	API TextureImpl* CreateTexture()
	{
		return (new TextureImpl());
	}
	API void DeleteTexture(TextureImpl* pObj)
	{
		delete pObj;
	}

	API TextureRenderPtr GetRenderFunction()
	{
		return &TextureImpl::Render;
	}
	API TextureGetPathPtr GetGetPathFunction()
	{
		return &TextureImpl::GetPath;
	}
	API TextureGetCountPtr GetCountFunction()
	{
		return &TextureImpl::GetCount;
	}
}