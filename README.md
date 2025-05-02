# Preview
![image](https://github.com/user-attachments/assets/abf166eb-b493-4e20-9b9b-3dea4e6647ad)

# Description
A C++ graphics rendering application using OpenGL and the GUI framework "dear ImGUI".

# Roadmap
## UI/Editor
- [x] Node-tree scene system, with the Scene as the root  
- [x] Hierarchy window, displaying and allowing interaction with the Node tree  
- [x] Inspector window, allowing modification of a Node or Node subclass  
- [x] Scene View window, displaying the rendered scene in a container  
- [x] Console window, displaying debugging information (WIP)
- [x] Highlight selected Nodes using stencil buffer
- [ ] Gizmos for Lights and Cameras to show their positions, with RGBA sprites  
- [ ] Project system for saving and loading progress  
## Scripting  
- [x] Script class, with a pure virtual update method
- [ ] Editor and Play mode distinction for Scripts
## Physics
- [x] Gravity physics
## Models
- [x] Assimp model loading
## Scene Elements
- [x] Skybox (cube-map texture rendered behind everything else)  

# Use
Note: This repository uses submodules, so please use --recurse-submodules when git-cloning.  

Build project files with cmake (cmake .) in the root directory.  
- cmake 3.12 or newer is required.

Example: Visual Studio 2022
- To run, change the startup project to Application, then press F5.  

# Modules Used
[Dear ImGui](https://github.com/ocornut/imgui)  
[GLFW](https://github.com/glfw/glfw)  
[GLAD](https://github.com/Dav1dde/glad)  
[learnopengl.com](https://learnopengl.com/)  
[stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)  
[Input](https://stackoverflow.com/questions/55573238/how-do-i-do-a-proper-input-class-in-glfw-for-a-game-engine)  

# Credits
The Cherno
