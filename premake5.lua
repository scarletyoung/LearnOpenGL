workspace "LearnOpenGL"
  architecture "x64"
  configurations {"Debug", "Release",  "Dist"}
  startproject "Editor"
  characterset("Unicode")
  defines {"G_HIGH_PRECISION"}

-- include directory
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
IncludeDir = {}
IncludeDir['glfw'] = "LearnOpenGL/third/glfw/include"
IncludeDir['glad'] = "LearnOpenGL/third/glad/include"
IncludeDir["stb_image"] = "LearnOpenGL/third/stb_image"
IncludeDir['glm'] = "LearnOpenGL/third/glm"

include "LearnOpenGL/third/glfw"
include "LearnOpenGL/third/glad"

project "LearnOpenGL"
  location "%{prj.name}"
  kind "ConsoleApp"
  language "C++"
  cppdialect "c++17"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  files {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
    "%{prj.name}/third/stb_image/**.cpp",
    "%{prj.name}/third/stb_image/**.h",
  }

  includedirs {
    "%{prj.name}/src",
    "%{IncludeDir.glfw}",
    "%{IncludeDir.glad}",
    "%{IncludeDir.stb_image}",
    "%{IncludeDir.glm}",
  }

  links {
    "glfw",
    "glad",
    "opengl32.lib",
  }

  defines {}

  filter "configurations:Debug"
    defines {"G_DEBUG", "G_ENABLE_ASSERTS"}
    symbols "On"
    runtime "Debug"
    staticruntime "on"

  filter "configurations:Release"
    defines {"G_RELEASE"}
    optimize "On"
    runtime "Release"
    staticruntime "on"

  filter "configurations:Dist"
    defines {"G_DIST"}
    optimize "On"
    runtime "Release"
    staticruntime "on"

  filter "system:windows"
    defines {
      "G_PLATFORM_WINDOWS"
    }

  filter "system:macos"
    defines {
      "G_PLATFORM_MAC"
    }