# Preview
![image](https://github.com/user-attachments/assets/cdc3609c-a13f-4393-b11c-93826cc0753b)

# Description
A C++ graphics rendering application using OpenGL and the GUI framework "dear ImGUI".

# Roadmap/Features
## Engine: Nodes
- [x] Scene node
- [x] Camera node
- [ ] Text rendering node
- [x] Audio source node
- [x] 2D Sprite node using stb_image
- [x] 3D Model node using Assimp
- [x] Skybox (cube-map texture rendered behind everything else)
- [x] Directional Light node
- [ ] Point Light node (untested)
## Engine: Utils
- [x] Event System for variadic generic events (e.g. `Event<Node*, int, int>`)
- [x] Factory + JSON serialisation
## Engine: Physics
- [x] Planetary gravity physics
## Engine: Lua Scripting
- [x] Basic interop Lua <-> C++
- [x] Node bindings
- [x] Camera bindings
- [x] KeyInput bindings
- [ ] Type hints / documentation (maybe autogen)
## Editor: UI
- [x] Node-tree scene system, with the Scene as the root  
- [x] Hierarchy window, displaying and allowing interaction with the Node tree  
- [x] Inspector window, allowing modification of a Node or Node subclass  
- [x] Scene View window, displaying the rendered scene in a container  
- [x] Console window, displaying debugging information
- [x] Highlight selected Nodes using stencil buffer
- [ ] Gizmos for Lights and Cameras to show their positions, with RGBA sprites (WIP)
- [x] Saving and loading of scenes

# Project Setup
- This repository uses submodules, so please use --recurse-submodules when git-cloning.    
- No binaries are located in this repo, so instead use premake5 to generate them.
- Premake binaries for Windows/Linux are included in the `premake` folder, but you may want/need to use your own version.  
- Example build command (Windows): `./premake/premake5 vs2022`.  

## Ubuntu
You will need to install dependencies yourself:
- `xargs sudo apt-get -y install < apt-packages.txt`  

To uninstall these dependencies, replace `install` in the
above command with `remove`, or `purge`.

# Project Building
- Build the `TankEditor` project for the Editor (UI).
- Build the `TankPlayer` project for the Standalone Player (no UI).

# References
### Tutorials
- The Cherno
- [learnopengl.com](https://learnopengl.com/)
### UI
- [Dear ImGui](https://github.com/ocornut/imgui)
### OpenGL
- OpenGL (Khronos)
- [GLFW](https://github.com/glfw/glfw)  
- [GLAD](https://github.com/Dav1dde/glad)
### Resource Loading
- [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
- [assimp](https://github.com/assimp/assimp)
- [ffmpeg](https://git.ffmpeg.org/ffmpeg.git)
### Logging
- [spdlog](https://github.com/gabime/spdlog)
### JSON / Serialisation
- [nlohmann/json](https://github.com/nlohmann/json)
### Tooling
- [premake](https://premake.github.io/)
### Misc
- [KeyInput](https://stackoverflow.com/questions/55573238/how-do-i-do-a-proper-input-class-in-glfw-for-a-game-engine)

