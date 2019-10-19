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
    
    