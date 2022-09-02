project "glad"
  kind "StaticLib"
  language "C"
    
  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  files
  {
    "include/glad/glad.h",
    "include/KHR/khrplatform.h",
    "src/glad.c"
  }

  includedirs {
    "include"
  }
    
  filter "system:windows"
    staticruntime "on"

  filter "configurations:Debug"
	runtime "Debug"
  staticruntime "on"
	symbols "on"

  filter "configurations:Release"
	runtime "Release"
    staticruntime "on"
	optimize "on"

  filter "configurations:Dist"
	runtime "Release"
    staticruntime "on"
	optimize "on"
    symbols "off"
