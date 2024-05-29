# DirectX11-Course

<img src="./DX11-logo.jpg">

Project generated while doing Sections 4-8 of the Udemy course *[The Complete Course of Microsoft DirectX 2024](https://www.udemy.com/course/directx-course/)* by 'The Tech Courses'.

The ['main' branch](https://github.com/AaronRuizMoraUK/DirectX11-Course/tree/main) contains a **Rendering Engine using DirectX 11** built from scratch following the course. It also contains additional improvements on top of the course content (see [DirectX Course Improvements](#DirectX-Course-Improvements) section).

The ['SimpleRenderer' branch](https://github.com/AaronRuizMoraUK/DirectX11-Course/tree/SimpleRenderer) contains a
**Simple DirectX 11 Renderer** that was generated during Sections 1-3 of the course.

<img src="./DX11-GraphicsEngine.gif">

## Build and Run

Requirements
- Graphics card with DirectX 11 support
- Microsoft Windows 
- Microsoft Visual Studio 2022
- CMake 3.28

Steps
- Clone repo
  ````
  git clone https://github.com/AaronRuizMoraUK/DirectX11-Course.git
  ````
- Generate Visual Studio solution using CMake in `build` folder
  ```` 
  cd DirectX11-Course
  mkdir build
  cd build
  cmake .. -G "Visual Studio 17 2022"
  ````
- Open `DirectX11-Course.sln` with Visual Studio
- Build and run `main` project

## Controls

Camera can be controlled while pressing the **right mouse button**:

- Use `WSAD` keys to move forward, backwards and sideways.
- Use `QE` keys to move upward and downwards
- Use mouse to rotate and pitch the camera.
- Use mouse wheel to speed up/down camera movement.

## Engine Architecture

The engine is divided in multiple projects.

| Project | Description |
| ------- | ----------- |
| **Core** | Library with **basic functionality** for the engine, like logging, debugging or the math data structures (vectors, matrices, transform, color, etc.) |
| **Graphics** | Library providing a generic graphics API encapsulating calls to DirectX 11 API. This is also known as the **Render Hardware Interface (RHI)**. The rest of the engine will communicate with this library and not with DirectX 11 directly. |
| **GraphicsTests** | Contains unit tests for Graphics library. |
| **Runtime** | This library contains more general constructs to build graphics applications, such as Window, Renderer, Camera or Assets. The renderer has an Scene with objects to render. |
| **EditorApplication** | Project with `main.cpp` that generates the executable. It creates a Window, a Renderer and a Camera using the Runtime library. Then it adds objects to the renderer's scene and starts the main loop, updating the camera and rendering the scene. |
| **Content** | This project contains the Assets folder, the main and 3rdParty CMake files and this *readme* file. |

## DirectX Course Improvements

I did the following improvements on top of the content provided in the DirectX 11 course:

- Used a **platform-independent math library** (**[mathfu](https://github.com/google/mathfu.git)**) instead of the DirectX Math library. This prevents the engine from being tied to a specific Graphics API, simplifying the effort to add support for other platforms in the future.
- Targets setup in CMake with 3 configurations:
  - **Debug**: No optimizations and debug information enabled. Asserts, logs and full debugging available.
  - **Development**: Optimizations and debug information enabled. Asserts, logs and partial debugging available with fast execution.
  - **Release**: Optimizations enabled and no debug information. Asserts, logs and debugging not available. Fastest execution.
- Fully **encapsulated DirectX 11** behind a separate library (Graphics). This simplifies the addition of other Graphics API in the future.
- Enabled **DirectX 11 Debug layer** when running in Debug configuration. Very useful to detect errors in DirectX and potential performance issues.
- Graphics device checks on destruction if there are device objects still being referenced and therefore leaking (missing to be destroyed).
- `SwapChain` supports ***Flip Discard*** technique with double buffering. This provides faster presentations since it flips between front and back buffer instead making a copy. The implementation allows any number of back buffers, default is 2.
- `FrameBuffer` supports both textures and typed buffers as render targets.
- Implemented the complete set of **Resource Views** (see **[DirectX 11 Resource table](https://github.com/AaronRuizMoraUK/DirectX12-Resources-Cheatsheet/blob/main/DirectX11.md)**)
    - `ShaderResourceView` supports multiple types of Textures (1D, 2D, 2DMS, 3D, Cube, Array) and Buffers (Typed, Structured, Raw).
    - `ShaderRWResourceView` (aka *Unordered Access View*) supports multiple types of Textures (1D, 2D, 3D, Array) and Buffers (Typed, Structured, Raw).
    - `RenderTargetView` supports multiple types of Textures (1D, 2D, 2DMS, 3D, Array) and Buffers (Typed)
    - `DepthStencilView` supports multiple types of Textures (1D, 2D, 2DMS, Array).
- `ShaderCompiler` is a separate class that generates shader bytecode and `Shader` will consume shader bytecode. This way `Shader` is not tied to an specific method of shader compilation, simplifying the effort to add support for *offline* shader compilation in the future.
- Assets only have generic data imported from the files in the Assets folder, they do not include DirectX or Graphics structures. For example, `TextureAsset` has a buffer (bytes) with the image imported from file, but it doesn't include a graphics' Texture. Another example is `MeshAsset`, it has the list of positions, indices, etc. imported from FBX or GLTF files, but it doesn't include a graphics' Buffer. This keeps the assets system nicely decoupled from graphics structures. Other classes (for example Renderer's `Object`) will use the assets, load their data and then construct their necessary structures from them.
- `Camera` only handles view/projection matrices and camera updates, it doesn't include any DirectX or Graphics structure. This means the camera isn't tied to graphics unnecessarily. Instead, the renderer's scene will get the view/projection matrices from the camera and it's the scene's responsibility to update the constant buffers for the shaders.

Unfortunately the course was taken down in Udemy while I was doing the last section :(, so I had to fill some gaps to bring everything together. I added the following to finish up the engine.

- In order to know what resources the shaders expect and in what slots they should be bound to, the shader compiler uses the reflection data from the shader to provide a `ShaderResourceLayout`.
- The `Pipeline` class (which has the shaders for each stage: vertex, pixel, etc.) uses the `ShaderResourceLayout` from each shader to create a `PipelineResourceBindings` object. The pipeline produces `PipelineResourceBindings` objects which can be filled with resources by slot or by name.
- `Renderer` handles the device, swap chain, main frame buffer and a scene.
- `PipelineObject` has the `Pipeline` and the binding of resources via several `PipelineResourceBindings` objects (per Scene, per Material, per Object).
- A `Scene` is what brings all together, handling a `PipelineObject`, the binding of resources and the drawing of all objects in the scene. It uses a `CommandList` to record all the commands asynchronously.
- `main.cpp` will create the Window, the Renderer, the Camera and the Objects. It'll get the Scene from the renderer and add the objects and the camera to it. Then in a loop it'll update the camera, render the scene and present.
- Implemented [Blinn-Phong illumination model](https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model) in Pixel Shader.
- Implemented [Normal mapping](https://en.wikipedia.org/wiki/Normal_mapping) to provide more detailed surfaces to objects.

## 3rdParty Libraries

- **[glfw](https://github.com/glfw/glfw.git)**: Provides a simple, platform-independent API for creating windows, contexts and surfaces, reading input, handling events, etc.
- **[mathfu](https://github.com/google/mathfu.git)**: Provides a simple and efficient math library with vectors, matrices and quaternions classes.
- **[stb](https://github.com/nothings/stb.git)**: Provides several single-file graphics and audio libraries for C/C++. Used for loading images.
- **[assimp](https://github.com/assimp/assimp.git)**: Library to load various 3D file formats into a shared, in-memory format. Used for loading 3D meshes from FBX and GLTF files.
