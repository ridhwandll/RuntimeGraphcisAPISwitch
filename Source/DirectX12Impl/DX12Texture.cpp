#include "../Engine/Texture.hpp" // I don't like this ../, but I am lazy
#include <iostream>

extern "C"
{
	API TextureImpl* CreateTexture()
	{
		std::cout << "Allocating DX12 Texture\n";
		return (new TextureImpl());
	}
	API void DeleteTexture(TextureImpl* pObj)
	{
		std::cout << "Deleting DX12 Texture\n";
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
		std::cout << "Rendering DX12\n";
	}
	void TextureImpl::Bind(int commandIndex)
	{
		std::cout << "DX12Texture::Bind, commandIndex" << commandIndex << std::endl;
	}
	int TextureImpl::GetCount()
	{
		return 2;
	}
}