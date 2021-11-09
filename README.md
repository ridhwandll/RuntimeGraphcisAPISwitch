# RuntimeGraphcisAPISwitch
It is common in Game Engines to switch the rendering API in runtime, but that requires the use of "interfaces" and the "interface" is literally pure virtual functions in C++, which are **slow**. Now wait a second, are they actually **slow**? What if we write our own way in which we don't use virtual functions, but simulate the way `virtual` works, but won't use the keyword `virtual`? It turns out, `virtual` functions are not *that* bad, infact it performes better from what I can write manually! I will go ahead and throw a benchmark towards your face:
```cpp
-----------------------------------------------------
Benchmark           Time             CPU   Iterations
-----------------------------------------------------
Virtual           180 ms          179 ms            9
Virtual           202 ms          202 ms           10
Virtual           191 ms          191 ms           10
Virtual           192 ms          192 ms           10
Virtual           185 ms          186 ms           10
Virtual           188 ms          188 ms           10
NonVirtual        192 ms          192 ms           10
NonVirtual        189 ms          189 ms           10
NonVirtual        201 ms          202 ms           10
NonVirtual        172 ms          172 ms           10
NonVirtual        191 ms          192 ms           10
NonVirtual        183 ms          183 ms           10
```
By looking at these, numbers, the difference can't be actually told. If you run the project several times after tweaking some parameters, you will see that `Virtual` is performing better in most cases. Things to note:

 - I am no expert, this project is basically me playing around with virtual and non virtual, if you spot any mistakes in my benchmarking/code you can report it to me at our [Discord](https://discord.gg/Tmp9TEBWwc) or open an Issue here!
 - This project uses [google/benchmark](https://github.com/google/benchmark) to measure the performance
 - How I can run the project? If you are a basic human, just clone the project recursively and use the batch file if you are on Windows(Best OS) and Shell script if you're on Linux. That will generate the project files inside a newly created folder named "build".
 - Wait, how you replace the virtual functions? By compiling the source in a SHARED library, then at program startup I manually load the functions pointers. In this project I compile 2 dummy SHARED library, one is `VulkanImpl` another is `DirectX12Impl`. They each produce a SHARED libary, which is loaded upon application startup depending on the API of choice. You get the drill... just look at the code for more information!
 - Thanks to [veridisquot](https://github.com/veridisquot) for adding Linux spupport!

## Conclusion
So what should I do? Should I use `virtual` or load a SHARED library? I would use `virtual`. Because I cannot optimize the calls from the SHARED library which is done via function pointers, if anyone can then please [let me know](https://discord.gg/Tmp9TEBWwc).