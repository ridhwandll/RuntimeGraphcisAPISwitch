#include "Texture.hpp"
#include <benchmark/benchmark.h>

// Bench Function
std::string Test(Texture* texture)
{
	std::string apath = texture->GetPath();
	std::string bpath = texture->Render();
	for (int i = 0; i < 20; i++)
		bpath = texture->Render();
	std::string cpath = apath + bpath;
	return cpath;
}

std::string VirtualTest(VirtualTexture* texture)
{
	std::string apath = texture->GetPath();
	std::string bpath = texture->Render();
	for (int i = 0; i < 20; i++)
		bpath = texture->Render();
	std::string cpath = apath + bpath;
	return cpath;
}

static void NonVirtual(benchmark::State& state)
{
	Texture textureNonVirtual;
	textureNonVirtual.Initialize();
	for (auto _ : state)
		Test(&textureNonVirtual);
	textureNonVirtual.Destroy();
}
static void Virtual(benchmark::State& state)
{
	VirtualTexture* textureVirtual;
	textureVirtual = CreateVirtualTexture();
	for (auto _ : state)
		VirtualTest(textureVirtual);
	delete textureVirtual;
}
BENCHMARK(Virtual);
BENCHMARK(NonVirtual);

int main(int argc, char* argv[])
{
	benchmark::Initialize(&argc, argv);
	if (benchmark::ReportUnrecognizedArguments(argc, argv))
		return 1;
	benchmark::RunSpecifiedBenchmarks();
	benchmark::Shutdown();
}