#include "Texture.hpp"
#include <benchmark/benchmark.h>
#define COUNT 1000

// Bench Function
std::string Test(Texture& textureNonVirtual)
{
	std::string apath = textureNonVirtual.GetPath();
	std::string bpath = textureNonVirtual.Render();
	for (int i = 0; i < COUNT; i++)
		bpath += textureNonVirtual.Render();

	return apath + bpath;
}

std::string VirtualTest(VirtualTexture* textureVirtual)
{
	std::string apath = textureVirtual->GetPath();
	std::string bpath = textureVirtual->Render();
	for (int i = 0; i < COUNT; i++)
		bpath += textureVirtual->Render();

	return apath + bpath;
}

static void NonVirtual(benchmark::State& s)
{
	Texture textureNonVirtual;
	textureNonVirtual.Initialize();

	for (auto _ : s)
		Test(textureNonVirtual);

	textureNonVirtual.Destroy();
}

static void Virtual(benchmark::State& s)
{
	VirtualTexture* textureVirtual;
	textureVirtual = CreateVirtualTexture();
	for (auto _ : s)
		VirtualTest(textureVirtual);
	delete textureVirtual;
}

BENCHMARK(Virtual)->Unit(benchmark::kMillisecond);
BENCHMARK(Virtual)->Unit(benchmark::kMillisecond);
BENCHMARK(Virtual)->Unit(benchmark::kMillisecond);
BENCHMARK(Virtual)->Unit(benchmark::kMillisecond);
BENCHMARK(Virtual)->Unit(benchmark::kMillisecond);
BENCHMARK(Virtual)->Unit(benchmark::kMillisecond);

BENCHMARK(NonVirtual)->Unit(benchmark::kMillisecond);
BENCHMARK(NonVirtual)->Unit(benchmark::kMillisecond);
BENCHMARK(NonVirtual)->Unit(benchmark::kMillisecond);
BENCHMARK(NonVirtual)->Unit(benchmark::kMillisecond);
BENCHMARK(NonVirtual)->Unit(benchmark::kMillisecond);
BENCHMARK(NonVirtual)->Unit(benchmark::kMillisecond);

int main(int argc, char* argv[])
{
	DLL_HANDLE handle;
#ifdef _WIN32
	if (GAPIType == RenderAPIType::Vulkan)
		handle = LoadLibrary("VulkanImpl.dll");
	else if (GAPIType == RenderAPIType::DirectX)
		handle = LoadLibrary("DirectX12Impl.dll");
#else
	if (GAPIType == RenderAPIType::Vulkan)
		handle = dlopen("./libVulkanImpl.so", RTLD_NOW);
	else if (GAPIType == RenderAPIType::DirectX)
		handle = dlopen("./libDirectX12Impl.so", RTLD_NOW);
#endif
	STexturePointers = TexturePointers(handle);

	benchmark::Initialize(&argc, argv);
	if (benchmark::ReportUnrecognizedArguments(argc, argv))
		return 1;
	benchmark::RunSpecifiedBenchmarks();
	benchmark::Shutdown();

#ifdef _WIN32
	FreeLibrary(handle);
#else
	dlclose(handle);
#endif
}