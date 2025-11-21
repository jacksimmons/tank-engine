project("Tank-ScriptCore" LANGUAGES CSharp) #DLL
    file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
        "Source/*.cs"
    )
    add_library(${PROJECT_NAME} SHARED
        ${PROJECT_SOURCES}
    )
    set_target_properties(${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "../Application/Resources/Scripts"
        DOTNET_TARGET_FRAMEWORK_VERSION 4.8
    )