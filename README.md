# diesel

Diesel is a single header stateless DirectX-11 renderer. You only need to include diesel.h in
project and that is all. There are only two dependencies:

* d3d11.lib
* dxgi.lib

## Basic concepts

The main concept diesel is heavily inspired by https://www.dropbox.com/sh/4uvmgy14je7eaxv/AABboa6UE5Pzfg3d9updwUexa?dl=0&preview=Designing+a+Modern+GPU+Interface.pdf.


## Getting Started

Please read the [Getting started](GettingStarted.md) for the "Hello world" of diesel.

## Resources

A resource is the gerenal name for textures or shaders or constant buffers and so on. 
Please note that all resources are managed within diesel. You have no direct access to the
underlying resources. The engine uses always a RID (resource identifier) to access or bind resources.
The lifecycle management is also handled by the engine. There is no need to free or delete any
resource. Everything is cleared when the engine shutdowns.

The following document [Creating resource](Resources.md) describes the API on how to create them.

## Examples

There are a lot of examples in the examples dircetory. Not all of these examples demonstrate certain feature.
Sometimes I just add something for the fun of it. The list will grow.
[Examples](Examples.md) is the 

## More examples

There is also a complete game developed with diesel. It is a simple arena shooter. You
can find here: [ds_arena](https://github.com/amecky/ds_arena)

## Debugging resources

Diesel supports one method to debug the resources and draw items. Please read [here](Debugging.md)

## GPU Profiler

diesel includes a simple GPU profiler. You can read about it [here](GPUProfiler.md)


## Math

A rendering engine uses vectors and matrices in some way. Therefore diesel comes with a 
template based Vector 2D/3D/4D and matrix implementation inside the header file. 
You can easily 

## Status

The engine itself is still in a beta state. I do not expect many changes to the API anymore.
But the state tracking system is not yet working 100% correct. So right now the engine is not completely
stateless since certain states might be leaking.

## Contact

In case you have found any bug or want to get in conact or have any question then please send an email to amecky@gmail.com.
Any kind of input is always welcome.