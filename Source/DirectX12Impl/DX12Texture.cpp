#include "../Engine/Texture.hpp" // I don't like this ../, but I am lazy
#include <iostream>

extern "C"
{
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
	API TextureBindPtr GetBindFunction()
	{
		return &TextureImpl::Bind;
	}
	API TextureGetCountPtr GetCountFunction()
	{
		return &TextureImpl::GetCount;
	}

	void TextureImpl::Render()
	{
	}

	void TextureImpl::Bind(int commandIndex)
	{
	}

	int TextureImpl::GetCount()
	{
		return 2;
	}
}