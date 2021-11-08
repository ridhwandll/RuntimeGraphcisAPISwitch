#include <iostream>
#include "Texture.hpp"

int main(int argc, char* argv[])
{
	SetAPI(RenderAPIType::DirectX);
	Texture texture;
	texture.Initialize();
	texture.Bind(8888);
	texture.Render();
	std::cout << "Texture::GetCount " << texture.GetCount() << "\n";
	texture.Destroy();
}