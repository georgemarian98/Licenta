# Scene Editor

Scene Editor is a Blender-like application that you can easily edit and modify objects and generate worlds.

# Install
For clonning, use the following command to load the submodules as well: 
```
git clone --recursive https://github.com/georgemarian98/Licenta.git
```

# Current state

The features supported by the applications are:

* Import .obj models
* Edit meshes properties
* Import a skybox
* Manipulate the light properties
* Export the scene in a .yaml format

After the scene was exported the user can easily include it in the user program. The include folder and the lib libraby must be included in the C++ project and the assimp DLL must be copied in the executable folder. Run-time library option of the project must be Multi-Threaded and C++17 or newer.

To use the scene in the project, first you need to import the scene with:
```
auto scene = SceneEditor::Serializer::ImportScene("path to the folder where the scene was exported");
```

Finally the scene must be drawn using:
```
scene->Draw(camera);
```
where camera is a SceneEditor::Camera object.

To manipulate the scene even more in code you must retrive the controller with the 
``` 
std::shared_ptr<ModelController> GetController(int Index) 
``` 
The controller exposes all the properties of the mesh.  