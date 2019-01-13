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
    location ("Projects/" .. _ACTION)
    configurations { "Debug", "Release" }

project "DirectXTutorial"
    location ("Projects/".. _ACTION .."/DirectXTutorial")
    kind "WindowedApp"
    language "C++"
    
    targetdir "Binaries/%{cfg.buildcfg}"
    
    files { "Source/*.h", 
            "Source/*.cpp",
            "Source/Framework/*.h",
            "Source/Framework/*.cpp" 
        }

    debugdir "./"

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
    
    