#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem>

#include <Windows.h>
#include <commdlg.h>

#include <string>
#include <sstream>
#include <streambuf>
#include <fstream>
#include <stdio.h>

#include <array>
#include <vector>
#include <unordered_map>

#include "stb_image.h"

#include "imgui.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <glad/glad.h> //OpenGL functions

#include <GLFW/glfw3.h> //window managing functions

#include <glm/glm.hpp> //math stuff
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

//My files
#include "Renderer/Mesh.h"
#include "Renderer/VertexData.h"

#include "OpenGL/Errors.h"
#include "OpenGL/SkyBox.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/Shader.h"

#include "core/Window.h"
#include "core/Camera.h"

std::ostream& operator<<(std::ostream& g, glm::vec3 vec);



