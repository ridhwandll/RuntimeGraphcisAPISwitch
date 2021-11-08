#include <iostream>
#include "Texture.hpp"
#include <chrono>
#include <benchmark/benchmark.h>

class Timer
{
public:
	Timer() { Reset(); }

	~Timer()
	{
	}

	void Reset() { mStart = std::chrono::high_resolution_clock::now(); }

	// Returns the elapsed time in seconds
	float Elapsed() { return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - mStart).count() * 0.001f * 0.001f * 0.001f; }

	float ElapsedMillis() { return Elapsed() * 1000.0f; }

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> mStart;
};

class VirtualTexture
{
public:
	virtual ~VirtualTexture() = default;
	virtual void Render() = 0;
	virtual void Bind(int commandIndex) = 0;
	virtual int GetCount() = 0;
};

class DX12VirtualTexture : public VirtualTexture
{
public:
	DX12VirtualTexture() = default;
	~DX12VirtualTexture() = default;
	virtual void Render() override {};
	virtual void Bind(int commandIndex) override {};
	virtual int GetCount() override { return 2; };
};

class VulkanVirtualTexture : public VirtualTexture
{
public:
	VulkanVirtualTexture() = default;
	~VulkanVirtualTexture() = default;
	virtual void Render() override {};
	virtual void Bind(int commandIndex) override {};
	virtual int GetCount() override { return 1; };
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

// Bench Function
void Test(Texture* texture)
{
	texture->Bind(8888);
	texture->Render();
}
void VirtualTest(VirtualTexture* texture)
{
	texture->Bind(8888);
	texture->Render();
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