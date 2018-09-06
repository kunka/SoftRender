# SoftRender
To learn OpenGL, implement basic graphics by OpenGL and SoftRender at the same time.

[![license](http://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/Tencent/xLua/blob/master/LICENSE.TXT)

### 2D line clip
<img src="screenshot/Rast2DLine.png" width="25%">2DLine VS Clip<img src="screenshot/Rast2DLine_Clip.png" width="25%">

### 3D Cube
<img src="screenshot/Cube_WF.png" width="25%">WireFrame VS TextureMap<img src="screenshot/Cube.png" width="25%">

### LightingMap
<img src="screenshot/LightingMap_GL.png" width="25%"> OpenGL VS SoftRender <img src="screenshot/LightingMap.png" width="25%">

### ShadowMap
<img src="screenshot/ShadowMap_GL.png" width="25%"> OpenGL VS SoftRender <img src="screenshot/ShadowMap.png" width="25%">

### Blending
<img src="screenshot/Blend_GL.png" width="25%"> OpenGL VS SoftRender <img src="screenshot/Blend.png" width="25%">

### ModelLoad
<img src="screenshot/ModelLoad_GL.png" width="25%"> OpenGL VS SoftRender <img src="screenshot/ModelLoad.png" width="25%">

### TextureFiltering
<img src="screenshot/TextureFilter_GL.png" width="25%"> OpenGL VS SoftRender <img src="screenshot/TextureFilter.png" width="25%">


## How To Build(Clion)

### Win10
Build Assimp + Zlib
CMake + MINGW32
cmake --build cmake-build-debug --target assimp -- -j 8
Enabled formats: 3DS B3D OBJ BLEND FBX 3D
Disabled formats: AC ASE ASSBIN ASSXML BVH COLLADA DXF CSM HMP IRRMESH IRR LWO LWS MD2 MD3 MD5 MDC MDL NFF NDO OFF OGRE OPENGEX PLY MS3D COB IFC XGL Q3D Q3BSP RAW SIB SMD STL TERRAGEN X GLTF 3MF
Configuring done

### OSX
CMake
Add CMAKE_BUILD_TYPE  Release

