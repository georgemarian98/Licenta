-- Premake file inspired from https://github.com/HectorPeeters/opengl_premake_boilerplate
workspace "Licenta"
   configurations { "Debug", "Release" }
   startproject "3DWorldBuilder"

   flags { "MultiProcessorCompile" }

   filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

   filter "configurations:Release"
        defines { "RELEASE" }
        optimize "Speed"
        flags { "LinkTimeOptimization" }

project "3DWorldBuilder"

   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   architecture "x86_64"

   outputdir = "../bin/"

   targetdir (outputdir .. "%{cfg.buildcfg}" )
   objdir (outputdir ..  "obj/%{cfg.buildcfg}" )
   
   files { 
      "src/**.h", 
      "src/**.hpp",
      "src/**.cpp", 
      "src/**.c" 
   }

   includedirs
	{
      "deps/glad/include/", 
      "deps/glfw/include/", 
      "deps/glm/",
      "deps/imgui/", 
      "deps/imgui/examples"
   }

   links { "GLFW", "GLM", "GLAD", "ImGui" }

   filter "system:linux"
        links { "dl", "pthread" }

        defines { "_X11" }
        
   filter "system:windows"
        defines { "_WINDOWS" }

include "deps/glfw.lua"
include "deps/glad.lua"
include "deps/glm.lua"
include "deps/imgui.lua"