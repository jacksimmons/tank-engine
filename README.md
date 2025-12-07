# Preview
![image](https://github.com/user-attachments/assets/cdc3609c-a13f-4393-b11c-93826cc0753b)

# Description
A C++ graphics rendering application using OpenGL and the GUI framework "dear ImGUI".

# Roadmap/Features
## Engine: Nodes
- [x] Scene node
- [x] Camera node
- [ ] Text rendering node
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
## Engine: C# Scripting  
- [x] Script class, with a pure virtual update method
- [ ] Editor and Play mode distinction for Scripts
- [ ] C# integration
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

# Project Building
- Build the `TankEditor` project for the Editor (UI).
- Build the `TankPlayer` project for the Standalone Player (no UI).

# Modules Used
[Dear ImGui](https://github.com/ocornut/imgui)  
[GLFW](https://github.com/glfw/glfw)  
[GLAD](https://github.com/Dav1dde/glad)  
[learnopengl.com](https://learnopengl.com/)  
[stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)  
[Input](https://stackoverflow.com/questions/55573238/how-do-i-do-a-proper-input-class-in-glfw-for-a-game-engine)  

# Credits
The Cherno
