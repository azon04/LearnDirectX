-- premake5.lua

-- Actions
newaction {
    trigger = "clean",
    description = "clean up project files",
    execute = function()
        os.rmdir("./Projects")
    end
}

workspace "DirectXTutorial"
    location ("./")
    configurations { "Debug", "Release" }

function setup_project()
    -- TODO this might want to group as library definition
    directSDKPath = os.getenv( "DXSDK_DIR" )
    sysincludedirs { directSDKPath.."Include" }
    syslibdirs { directSDKPath.."Lib/x86" }
    -- END TODO

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
end

project "Tutorial 16 - Frustum Culling"
    location ("Projects/".. _ACTION .."/DirectXTutorial")
    kind "WindowedApp"
    language "C++"
    
    targetdir "Binaries/%{cfg.buildcfg}"
    
    files { 
            "Source/Main.cpp",
            "Source/Framework/*.h",
            "Source/Framework/*.cpp",
            "Source/Tutorial 16 - Frustum Culling/*.h",
            "Source/Tutorial 16 - Frustum Culling/*.cpp"
        }

    debugdir "./"

    includedirs  { "Source/Framework",  "Source/Tutorial 16 - Frustum Culling" }

    setup_project()

project "Tutorial 17 - Multitexturing"
    location ("Projects/".. _ACTION .."/DirectXTutorial")
    kind "WindowedApp"
    language "C++"
    
    targetdir "Binaries/%{cfg.buildcfg}"
    
    files { 
            "Source/Main.cpp",
            "Source/Framework/*.h",
            "Source/Framework/*.cpp",
            "Source/Tutorial 17 - Multitexturing/*.h",
            "Source/Tutorial 17 - Multitexturing/*.cpp"
        }

    debugdir "./"
    
    includedirs  { "Source/Framework",  "Source/Tutorial 17 - Multitexturing" }

    setup_project()
	
project "Tutorial 18 - Light Maps"
    location ("Projects/".. _ACTION .."/DirectXTutorial")
    kind "WindowedApp"
    language "C++"
    
    targetdir "Binaries/%{cfg.buildcfg}"
    
    files { 
            "Source/Main.cpp",
            "Source/Framework/*.h",
            "Source/Framework/*.cpp",
            "Source/Tutorial 18 - Light Maps/*.h",
            "Source/Tutorial 18 - Light Maps/*.cpp"
        }

    debugdir "./"
    
    includedirs  { "Source/Framework",  "Source/Tutorial 18 - Light Maps" }

    setup_project()

project "Tutorial 19 - Alpha Mapping"
    location ("Projects/".. _ACTION .."/DirectXTutorial")
    kind "WindowedApp"
    language "C++"
    
    targetdir "Binaries/%{cfg.buildcfg}"
    
    files { 
            "Source/Main.cpp",
            "Source/Framework/*.h",
            "Source/Framework/*.cpp",
            "Source/Tutorial 19 - Alpha Mapping/*.h",
            "Source/Tutorial 19 - Alpha Mapping/*.cpp"
        }

    debugdir "./"
    
    includedirs  { "Source/Framework", "Source/Tutorial 19 - Alpha Mapping" }

    setup_project()

project "Tutorial 20 - Bump Map"
    location ("Projects/".. _ACTION .."/DirectXTutorial")
    kind "WindowedApp"
    language "C++"
    
    targetdir "Binaries/%{cfg.buildcfg}"
    
    files { 
            "Source/Main.cpp",
            "Source/Framework/*.h",
            "Source/Framework/*.cpp",
            "Source/Tutorial 20 - Bump Map/*.h",
            "Source/Tutorial 20 - Bump Map/*.cpp"
        }

    debugdir "./"
    
    includedirs  { "Source/Framework", "Source/Tutorial 20 - Bump Map" }

    setup_project()

project "Tutorial 21 - Specular Map"
    location ("Projects/".. _ACTION .."/DirectXTutorial")
    kind "WindowedApp"
    language "C++"
    
    targetdir "Binaries/%{cfg.buildcfg}"
    
    files { 
            "Source/Main.cpp",
            "Source/Framework/*.h",
            "Source/Framework/*.cpp",
            "Source/Tutorial 21 - Specular Map/*.h",
            "Source/Tutorial 21 - Specular Map/*.cpp"
        }

    debugdir "./"
    
    includedirs  { "Source/Framework", "Source/Tutorial 21 - Specular Map" }

    setup_project()

project "Tutorial 22 - Render to Texture"
    location ("Projects/".. _ACTION .."/DirectXTutorial")
    kind "WindowedApp"
    language "C++"
    
    targetdir "Binaries/%{cfg.buildcfg}"
    
    files { 
            "Source/Main.cpp",
            "Source/Framework/*.h",
            "Source/Framework/*.cpp",
            "Source/Tutorial 22 - Render to Texture/*.h",
            "Source/Tutorial 22 - Render to Texture/*.cpp"
        }

    debugdir "./"
    
    includedirs  { "Source/Framework", "Source/Tutorial 22 - Render to Texture" }

    setup_project()

project "Tutorial 23 - Fog"
    location ("Projects/".. _ACTION .."/DirectXTutorial")
    kind "WindowedApp"
    language "C++"
    
    targetdir "Binaries/%{cfg.buildcfg}"
    
    files { 
            "Source/Main.cpp",
            "Source/Framework/*.h",
            "Source/Framework/*.cpp",
            "Source/Tutorial 23 - Fog/*.h",
            "Source/Tutorial 23 - Fog/*.cpp"
        }

    debugdir "./"
    
    includedirs  { "Source/Framework", "Source/Tutorial 23 - Fog" }

    setup_project()

project "Tutorial 24 - Clipping Planes"
    location ("Projects/".. _ACTION .."/DirectXTutorial")
    kind "WindowedApp"
    language "C++"
    
    targetdir "Binaries/%{cfg.buildcfg}"
    
    files { 
            "Source/Main.cpp",
            "Source/Framework/*.h",
            "Source/Framework/*.cpp",
            "Source/Tutorial 24 - Clipping Planes/*.h",
            "Source/Tutorial 24 - Clipping Planes/*.cpp"
        }

    debugdir "./"
    
    includedirs  { "Source/Framework", "Source/Tutorial 24 - Clipping Planes" }

    setup_project()

project "Tutorial 25 - Texture Transition"
    location ("Projects/".. _ACTION .."/DirectXTutorial")
    kind "WindowedApp"
    language "C++"
    
    targetdir "Binaries/%{cfg.buildcfg}"
    
    files { 
            "Source/Main.cpp",
            "Source/Framework/*.h",
            "Source/Framework/*.cpp",
            "Source/Tutorial 25 - Texture Transition/*.h",
            "Source/Tutorial 25 - Texture Transition/*.cpp"
        }

    debugdir "./"
    
    includedirs  { "Source/Framework", "Source/Tutorial 25 - Texture Transition" }

    setup_project()

project "Tutorial 26 - Transparency"
    location ("Projects/".. _ACTION .."/DirectXTutorial")
    kind "WindowedApp"
    language "C++"
    
    targetdir "Binaries/%{cfg.buildcfg}"
    
    files { 
            "Source/Main.cpp",
            "Source/Framework/*.h",
            "Source/Framework/*.cpp",
            "Source/Tutorial 26 - Transparency/*.h",
            "Source/Tutorial 26 - Transparency/*.cpp"
        }

    debugdir "./"
    
    includedirs  { "Source/Framework", "Source/Tutorial 26 - Transparency" }

    setup_project()

project "Tutorial 27 - Reflection"
    location ("Projects/".. _ACTION .."/DirectXTutorial")
    kind "WindowedApp"
    language "C++"
    
    targetdir "Binaries/%{cfg.buildcfg}"
    
    files { 
            "Source/Main.cpp",
            "Source/Framework/*.h",
            "Source/Framework/*.cpp",
            "Source/Tutorial 27 - Reflection/*.h",
            "Source/Tutorial 27 - Reflection/*.cpp"
        }

    debugdir "./"
    
    includedirs  { "Source/Framework", "Source/Tutorial 27 - Reflection" }

    setup_project()

group "Tools"

project "ObjParser"
    location ("Projects/".. _ACTION .. "/Tools" )
    kind "ConsoleApp"
    language "C++"
    
    targetdir "Binaries/Tools"

    files { "Tools/ObjParser/*.h",
            "Tools/ObjParser/*.cpp" }
    
    debugdir "./"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
    
    